/** @defgroup pibuiltin
 *
 *  @brief Provides builtin lambdas: for example car, cdr
 *
 */
/** @addtogroup pibuiltin */
/*@{*/
#ifndef PIBUILTIN_H
#define PIBUILTIN_H
#include "picell.h"
#include "pichecks.h"
#include "pilisp.h"
#include "pistack.h"
#include "piutils.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// ==================== Basic inline apply ====================
// not usable in the interpreter: no checks! => use only in the eval
#if INLINE_FUNCTIONS
inline cell *caar(cell *c) { return c->car->car; }
inline cell *cddr(cell *c) { return c->cdr->cdr; }
inline cell *cadr(cell *c) { return c->cdr->car; }
inline cell *cdar(cell *c) { return c->car->cdr; }
inline cell *cadar(cell *c) { return c->car->cdr->car; }
inline cell *caddr(cell *c) { return c->cdr->cdr->car; }
#else
cell *caar(cell *c);
cell *cddr(cell *c);
cell *cadr(cell *c);
cell *cdar(cell *c);
cell *cadar(cell *c);
cell *caddr(cell *c);
#endif

// ==================== Basic apply ====================
// differences from the first basic block: these functions can be called from
// the apply, because they do cell_remove and cell_push and check for args error
cell *builtin_car(cell *args);
cell *builtin_cdr(cell *args);
cell *builtin_cons(cell *args);
cell *builtin_atom(cell *args);
cell *builtin_eq(cell *args);

// ==================== Arithmetic ====================
cell *addition(cell *numbers);
cell *subtraction(cell *numbers);
cell *subtraction_invert_result(cell *numbers);
cell *subtraction_two_or_more_numbers(cell *numbers);
cell *multiplication(cell *numbers);
cell *division(cell *numbers);

// ==================== Logic ====================
cell * or (cell * operands);
cell * and (cell * operands);
cell * not(cell * operands);

// ==================== Comparison ====================
cell *greater(cell *operands);
cell *compare_greater_numbers(cell *first_num, cell *second_num);
cell *compare_greater_strings(cell *first_str, cell *second_str);

cell *greater_eq(cell *operands);
cell *compare_greater_eq_numbers(cell *first_num, cell *second_num);
cell *compare_greater_eq_strings(cell *first_str, cell *second_str);

cell *less(cell *operands);
cell *compare_less_numbers(cell *first_num, cell *second_num);
cell *compare_less_strings(cell *first_str, cell *second_str);

cell *less_eq(cell *operands);
cell *compare_less_eq_numbers(cell *first_num, cell *second_num);
cell *compare_less_eq_strings(cell *first_str, cell *second_str);

cell *integerp(cell *arg);
cell *symbolp(cell *arg);

// ==================== Lists ====================
cell *list(cell *args);
cell *reverse(cell *args);
cell *member(cell *args);
cell *nth(cell *args);
cell *concatenate(cell *args); // ! works only with strings
cell *append(cell *args);

cell *length(cell *args);
cell *length_string(cell *string);
cell *length_cons(cell *list);

cell *subseq(cell *args); // ! works only with strings
cell *subseq_one_index(cell *args, int start_index);
cell *subseq_two_indices(cell *args, int start_index);

// ==================== Utility ====================
cell *set(cell *args);
cell *set_change_existing_value(cell *args, cell *pair);
cell *set_add_new_value(cell *args, cell *prec);

cell *write(cell *arg);
cell *load(cell *arg, cell *env);
cell *bye(cell *arg);

// ==================== Macros ====================
cell *quote(cell *args, cell *env);
cell *cond(cell *arg, cell *env);
cell *setq(cell *args, cell *env);
cell *defun(cell *args, cell *env);
cell *let(cell *args, cell *env);
cell *dotimes(cell *arg, cell *env);
cell *map(cell *args, cell *env);
cell *timer(cell *arg, cell *env);

// ==================== Pilisp special functions ====================
cell *compile(cell *c, cell *env); // ! the compiler needs to be loaded
bool should_be_compiled(cell *to_compilate);
cell *asm_call(cell *args, cell *env);
cell *mem_dump(cell *arg);
cell *env(cell *arg);
cell *collect_garbage_call(cell *arg);

// ==================== Basic Lisp functions ====================
// works also on lists: eq does not, but 'it's slower
bool total_eq(cell *c1, cell *c2);

#if !INLINE_FUNCTIONS

cell *car(cell *c);
cell *cdr(cell *c);
cell *cons(cell *car, cell *cdr);
int atom(cell *c);
bool eq(cell *v1, cell *v2);

#else

inline cell *cons(cell *car, cell *cdr) { return mk_cons(car, cdr); }

inline int atom(cell *c) {
  return (c == NULL) ||
         (c->type == TYPE_SYM || c->type == TYPE_NUM || c->type == TYPE_STR ||
          c->type == TYPE_BUILTINLAMBDA || c->type == TYPE_BUILTINMACRO ||
          c->type == TYPE_KEYWORD);
}

inline bool eq(cell *v1, cell *v2) {
  if (!v1 || !v2)
    return (v1 == v2);
  if (is_num(v1) && is_num(v2))
    return (v1->value == v2->value);
  if (is_str(v1) && is_str(v2))
    return (strcmp(v1->str, v2->str) == 0);
  return (v1 == v2);
}

inline cell *car(cell *c) {
  if (c == NULL)
    return NULL;
#if CHECKS
  if (atom(c))
    pi_lisp_error("car applied to an atom");
#endif
  return c->car;
}

inline cell *cdr(cell *c) {
  if (c == NULL)
    return NULL;
#if CHECKS
  if (atom(c))
    pi_lisp_error("cdr applied to an atom");
#endif
  return c->cdr;
}

#endif

#endif // !PIBUILTIN_H
       /*@}*/