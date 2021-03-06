/** @defgroup picell
 *
 *  @brief Provides the data structures for LISP, like cells
 *
 */

/** @addtogroup picell */
/*@{*/
#ifndef PICELL_H
#define PICELL_H
#include "pisettings.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/********************************************************************************
 *                                  CELL DEFINITION
 ********************************************************************************/

/**
 * @brief Identifies the type of a cell
 *
 */
enum {
  TYPE_CONS = 0, // is a cell with a car and a cdr
  TYPE_SYM,      // symbol cell
  TYPE_NUM,      // integer cell
  TYPE_STR,      // string cell
  TYPE_FREE, // free cell: can be allocated from the memory manager to another
             // cell
  TYPE_BUILTINLAMBDA, /// represents a builtin lisp function (e.g. car, cdr...)
  TYPE_BUILTINMACRO,  /// represents a builtin lisp macro (e.g. dotimes,
                      /// defun...)
  TYPE_KEYWORD,       /// keyword cell (e.g. :mykeyword...)
};

/**
 * @brief Basic Lisp entity
 *
 * Every cell has a type identifier (referred to the type enum). It identifies
 * the type of the cell. `marked` and `marks` are fields used to collect garbage. 
 *
 */
typedef struct cell {
  unsigned char type;   /// type of the cell referred to the type enum
  unsigned char marked; /// 1 if marked in the "mark" phase of the gc
  unsigned long marks;  /// number of cells that refer to this cell
  union {
    int value; /// value of the num cell
    char *str; /// string of the string cell
    struct cell *
        next_free_cell; /// pointer to the next free cell for cells of type free
    struct {
      struct cell *car; /// car of the cons cell
      struct cell *cdr; /// cdr of the cons cell
    };
    struct {
      char *sym; // symbol for symbol cells
      union {
        struct {
          struct cell *(*bl)(
              struct cell *args); /// pointer to builtin lambda function for
                                  /// builtin lambdas
          void (*bs)(
              size_t stack_base,
              unsigned char
                  nargs); /// pointer to builtin stack lambdas function for
                          /// functions that have a stack implementation that
                          /// can be interpreted by the virtual machine
        };
        struct cell *(*bm)(
            struct cell *args,
            struct cell *env); /// pointer to builtin macro function
      };
    };
  };
} cell;

// unsafe unmark: no checks if cell is empty or a builtin! use only if you are
// sure that cell exists and is not a builtin symbol. it's faster
#if INLINE_FUNCTIONS
inline void unsafe_cell_remove(cell *c) { c->marks--; }
#else
void unsafe_cell_remove(cell *c);
#endif

/********************************************************************************
 *                                     STACK DEFINITION
 ********************************************************************************/

size_t stack_pointer;
cell *stack[STACK_LIMIT];

/********************************************************************************
 *                                  GARBAGE COLLECTOR
 ********************************************************************************/

// cells array
typedef struct {
  size_t block_size;
  cell *block;
} cell_block;

cell_block *cell_block_create(size_t s);
void cell_block_free(cell_block *cb);

// cells space: array of cell blocks. Just one of this will be instantiated:
// the pointer "memory" that represents the allocated cells in the interpreter
typedef struct {
  size_t cell_space_size;
  size_t cell_space_capacity;
  size_t n_cells;
  size_t n_free_cells;
  cell *first_free;
  cell *global_env;
  cell_block *blocks;
} cell_space;

cell_space *memory;
void init_memory();
void free_memory();

cell_space *cell_space_create();
cell *cell_space_get_cell(cell_space *cs);
cell *cell_space_is_symbol_allocated(cell_space *cs, char *symbol);
void cell_space_init(cell_space *cs);
void cell_space_grow(cell_space *cs);
void cell_space_double_capacity_if_full(cell_space *cs);
void cell_space_mark_cell_as_free(cell_space *cs, cell *c);
void cell_space_free(cell_space *cs);
bool cell_space_is_full(cell_space *cs);

/********************************************************************************
 *                                CELL BASIC OPERATIONS
 ********************************************************************************/

#if INLINE_FUNCTIONS
inline cell *get_cell() { return cell_space_get_cell(memory); }

inline cell *mk_num(int n) {
  cell *c = get_cell();
  c->type = TYPE_NUM;
  c->value = n;
  return c;
}

inline cell *mk_str(char *s) {
  cell *c = get_cell();
  c->type = TYPE_STR;
  c->str = malloc(strlen(s) + 1);
  strcpy(c->str, s);

  return c;
}

inline cell *mk_cons(cell *car, cell *cdr) {
  cell *c = get_cell();
  c->type = TYPE_CONS;
  c->car = car;
  c->cdr = cdr;
  return c;
}
#else
cell *get_cell();
cell *mk_num(int n);
cell *mk_str(char *s);
cell *mk_cons(cell *car, cell *cdr);
#endif

cell *mk_sym(char *symbol);
cell *mk_builtin_lambda(char *symbol, cell *(*function)(cell *),
                        void (*builtin_stack)(size_t, unsigned char));
cell *mk_builtin_macro(char *symbol, cell *(*function)(cell *, cell *));

cell *copy_cell(cell *c);
void free_cell_pointed_memory(cell *c);

/********************************************************************************
 *                                CELL IDENTIFICATION
 ********************************************************************************/
#if INLINE_FUNCTIONS
inline bool is_num(cell *c) { return c->type == TYPE_NUM; }
inline bool is_str(cell *c) { return c->type == TYPE_STR; }
inline bool is_cons(cell *c) { return c->type == TYPE_CONS; }
inline bool is_keyword(cell *c) { return c->type == TYPE_KEYWORD; }
inline bool is_sym(cell *c) {
  return c->type == TYPE_SYM || c->type == TYPE_BUILTINLAMBDA ||
         c->type == TYPE_BUILTINMACRO || c->type == TYPE_KEYWORD;
}
inline bool is_builtin(cell *c) {
  return c->type == TYPE_BUILTINLAMBDA || c->type == TYPE_BUILTINMACRO;
}
inline bool is_builtin_lambda(cell *c) { return c->type == TYPE_BUILTINLAMBDA; }
inline bool is_builtin_macro(cell *c) { return c->type == TYPE_BUILTINMACRO; }
#else
bool is_num(cell *c);
bool is_str(cell *c);
bool is_cons(cell *c);
bool is_sym(cell *c);
bool is_keyword(cell *c);
bool is_builtin(cell *c);
bool is_builtin_lambda(cell *c);
bool is_builtin_macro(cell *c);
#endif
cell *is_symbol_builtin_lambda(char *symbol);
cell *is_symbol_builtin_macro(char *symbol);
bool cell_is_in_global_env(cell *global_env, cell *c);

/********************************************************************************
 *                                  CELL PROTECTION
 ********************************************************************************/

#if INLINE_FUNCTIONS
inline void cell_push(cell *val) {
#if COLLECT_GARBAGE
  val->marks++;
#endif
}

inline void cell_remove(cell *val) {
#if COLLECT_GARBAGE
  if (!val || is_builtin(val))
    return;
  if (val->marks > 0)
    val->marks--;
#endif
}
#else
void cell_push(cell *val);
void cell_remove(cell *val);
#endif
void cell_push_recursive(cell *c); // mark as used
void cell_remove_recursive(cell *c);
void cell_remove_args(cell *args);
void cell_remove_pairlis(cell *new_env, cell *old_env);
void cell_remove_cars(cell *list);
void cell_remove_pairlis_deep(cell *new_env, cell *old_env);

/********************************************************************************
 *                                 CORE OF THE GC
 ********************************************************************************/

void collect_garbage(cell_space *cs);
void deep_collect_garbage(cell_space *cs);
void mark_memory(cell_space *cs);
void mark(cell *root);
void sweep(cell_space *cs);
void deep_sweep(cell_space *cs);

#endif // !PICELL_H
       /*@}*/