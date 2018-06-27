/** @defgroup picell
 *
 *  @brief Provides the data structures for LISP, like cells
 *
 */

/** @addtogroup picell */
/*@{*/

#ifndef PICELL_H
#define PICELL_H
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief enumeration to identify the type of one cell
 *
 */
enum {
  TYPE_CONS = 0, ///< cons cell: it has car and cdr
  TYPE_SYM,      ///< symbol cell
  TYPE_NUM,      ///< number cell
  TYPE_STR,      ///< string cell
  TYPE_FREE
  //   TYPE_KEYWORD,
  //   TYPE_BUILTINLAMBDA,
  //   TYPE_BUILTINMACRO,
  //   TYPE_BUILTINSTACK,
  //   TYPE_CXR,
};

/**
 * @brief type to describe a generic cell
 *
 */
typedef struct cell {
  unsigned char type, marked;
  union {
    struct {
      struct cell *car;
      struct cell *cdr;
    };
    char *sym;
    int value;
    char *str;
    struct cell *next_free_cell;
  };
  // union {
  //   double dvalue;
  //   long long int lvalue;
  // } numV;
} cell;

void init_memory();

/**
 * @brief function to get a cell
 *
 * @return cell* pointer to the new cell
 */
cell *get_cell();

/**
 * @brief make a number cell
 *
 * @param n the number
 * @return cell* pointer to the new cell
 */
cell *mk_num(int n);

/**
 * @brief make a string cell
 *
 * @param s the string
 * @return cell* pointer to the new cell
 */
cell *mk_str(const char *s);

/**
 * @brief make a new symbol cell
 *
 * @param symbol name of the symbol
 * @return cell* pointer to the new cell
 */
cell *mk_sym(const char *symbol);

/**
 * @brief make a new cons cell
 *
 * @param car pointer to the car
 * @param cdr pointer to the cdr
 * @return cell* pointer to the new cell
 */
cell *mk_cons(cell *car, cell *cdr);

cell *copy_cell(const cell *c);

int is_num(const cell *c);
int is_str(const cell *c);
int is_sym(const cell *c);
int is_cons(const cell *c);

// free the memory pointed by the cell. for example the string for str cells.
// does nothing if the cell has not pointers
void free_cell_pointed_memory(cell *c);

void cell_push(cell * c); // mark as used
void cell_remove(cell * c); // mark as not used

/********************************************************************************
 *                              STACK GARBAGE COLLECTOR
 ********************************************************************************/

typedef struct cell_stack_node{
  struct cell_stack_node * prec;
  struct cell_stack_node * next;
  cell * c;
} cell_stack_node;

typedef struct {
  cell_stack_node * head;
  cell_stack_node * tail;
} cell_stack;

cell_stack *cell_stack_create();
cell_stack_node *cell_stack_node_create_node(cell * val, cell_stack_node * next, cell_stack_node * prec);

void cell_stack_push(cell_stack * stack, cell * val);
void cell_stack_remove(cell_stack * stack, cell * val);

/********************************************************************************
 *                                  GARBAGE COLLECTOR
 ********************************************************************************/

// cells array
typedef struct {
  size_t block_size;
  cell *block;
} cell_block;

cell_block *new_cell_block(size_t s);

// cells space: array of cell blocks. Just one of this will be instantiated: the
// pointer "memory" that represents the allocated cells in the interpreter
typedef struct {
  size_t cell_space_size;
  size_t cell_space_capacity;
  size_t n_cells;
  size_t n_free_cells;
  cell_block *blocks;
  cell *first_free;
  cell_stack * stack;
} cell_space;

// allocates a new block and links the last free cell with the first free in the
// cell space
void cell_space_grow(cell_space *cs);
// doubles the capacity of the cell spce
void cell_space_double_capacity_if_full(cell_space *cs);
// always use this on one allocated cell space before use
void cell_space_init(cell_space *cs);
bool cell_space_is_full(const cell_space *cs);
// ALWAYS returns a new cell: if none is present it allocates new space
cell *cell_space_get_cell(cell_space *cs);
// checks if the symbol is present in the cell space
cell *cell_space_is_symbol_allocated(cell_space *cs, const char *symbol);
// marks the cell as free and updates the first cel, in the cs. cell must be in
// the cell space
void cell_space_mark_cell_as_free(cell_space *cs, cell *c);

// global var that represents the memory of the program
cell_space *memory;

void collect_garbage(cell_space *cs);
void mark(cell *root);
void sweep(cell_space *cs);

// !TODO: give the chance to free the mem

#endif // !PICELL_H

/*@}*/