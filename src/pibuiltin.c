#include "pibuiltin.h"
#include "pierror.h"

int atom(const cell *c) {
  return (c == NULL) // NIL case
         || (c->type == TYPE_SYM || c->type == TYPE_NUM || c->type == TYPE_STR);
}

bool eq(const cell *v1, const cell *v2) {
  if (!v1 || !v2)
    return (v1 == v2);
  if (is_num(v1) && is_num(v2))
    return (v1->value == v2->value);
  if (is_str(v1) && is_str(v2))
    return (strcmp(v1->str, v2->str) == 0);
  return (v1 == v2);
}

cell *addition(const cell *numbers) {
  long result = 0;
  const cell *act = numbers;
  while (act) {
    if (!is_cons(act))
      pi_error(LISP_ERROR, "impossible to perform addition");
    if (!is_num(car(act)))
      pi_error(LISP_ERROR, "added a non-number");
    result += car(act)->value;
    act = cdr(act);
  }
  return mk_num(result);
}

cell *subtraction(const cell *numbers) {
  if (!numbers || !cdr(numbers))
    // we need 2 numbers at least
    pi_error_args();

  if (!is_cons(numbers) || !is_cons(cdr(numbers)))
    pi_error(LISP_ERROR, "impossible to perform subtraction");
  if (!is_num(car(numbers)) || !is_num(car(cdr(numbers))))
    pi_error(LISP_ERROR, "subtracted a non-number");
  long result = car(numbers)->value;
  const cell *act = cdr(numbers);
  while (act) {
    if (!is_cons(act))
      pi_error(LISP_ERROR, "impossible to perform subtraction");
    if (!is_num(car(act)))
      pi_error(LISP_ERROR, "subtracted a non-number");
    result -= car(act)->value;
    act = cdr(act);
  }
  return mk_num(result);
}

cell *multiplication(const cell *numbers) {
  long result = 1;
  const cell *act = numbers;
  while (act) {
    if (!is_cons(act))
      pi_error(LISP_ERROR, "impossible to perform multiplication");
    if (!is_num(car(act)))
      pi_error(LISP_ERROR, "multiplicated a non-number");
    result *= car(act)->value;
    act = cdr(act);
  }
  return mk_num(result);
}

cell *division(const cell *numbers) {
  if (!numbers || !cdr(numbers))
    // we need 2 numbers at least
    pi_error_args();

  if (!is_cons(numbers) || !is_cons(cdr(numbers)))
    pi_error(LISP_ERROR, "impossible to perform division");
  if (!is_num(car(numbers)) || !is_num(car(cdr(numbers))))
    pi_error(LISP_ERROR, "divided a non-number");
  double result = (double)car(numbers)->value;
  const cell *act = cdr(numbers);
  while (act) {
    if (!is_cons(act))
      pi_error(LISP_ERROR, "impossible to perform division");
    if (!is_num(car(act)))
      pi_error(LISP_ERROR, "divided a non-number");
    if (car(act)->value == 0)
      pi_error(LISP_ERROR, "division by 0");
    result /= (double)car(act)->value;
    act = cdr(act);
  }
  return mk_num(result);
}

cell *car(const cell *c) {
  if (c == NULL)
    // (car NIL)
    return NULL;
  if (atom(c))
    pi_error(LISP_ERROR, "car applied to an atom");
  return c->car;
}
cell *cdr(const cell *c) {
  if (c == NULL)
    // (cdr NIL)
    return NULL;
  if (atom(c))
    pi_error(LISP_ERROR, "cdr applied to an atom");
  return c->cdr;
}
cell *caar(const cell *c) { return car(car(c)); }
cell *cddr(const cell *c) { return cdr(cdr(c)); }
cell *cadr(const cell *c) { return car(cdr(c)); }
cell *cdar(const cell *c) { return cdr(car(c)); }
cell *cadar(const cell *c) { return car(cdr(car(c))); }
cell *caddr(const cell *c) { return car(cdr(cdr(c))); }
cell *cons(const cell *car, const cell *cdr) { return mk_cons(car, cdr); }

cell *set(cell *name, cell *val, cell **env) {
  cell *prec = NULL;
  cell *act = *env;
  while (act) {
    if (eq(name, caar(act))) {
      // found
      car(act)->cdr = val;
      return cdar(act);
    }
    // iterate
    prec = act;
    act = cdr(act);
  }
  cell *new = cons(cons(name, val), NULL);
  if (prec)
    prec->cdr = new;
  else
    (*env) = new;
  return val;
  // return new;
}

cell *load(cell *name, cell **env) {
  if (!name || !is_str(name))
    pi_error(LISP_ERROR, "first arg must me a string");
  FILE *file = fopen(name->str, "r");
  if (!file)
    pi_error(LISP_ERROR, "can't find file");
  cell *val = NULL;
  while (!feof(file)) {
    cell *sexpr = read_sexpr(file);
    val = eval(sexpr, env);
  }
  return symbol_true;
}

cell *timer(const cell *to_execute, cell **env) {
  clock_t t1, t2;
  long elapsed;

  t1 = clock();
  cell *valued = eval(to_execute, env);
  t2 = clock();

  elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
  printf("time: %ld ms\n", elapsed);
  return valued;
}

cell * or (const cell *operands) {
  cell *act = operands;
  cell *atom = car(act);
  while (act) {
    if (atom)
      return atom;
    act = cdr(act);
    atom = car(act);
  }
  return NULL;
}
cell * and (const cell *operands) {
  cell *act = operands;
  cell *prev = NULL;
  cell *atom = car(act);
  while (act) {
    if (!atom)
      // NIL found
      return atom;
    prev = act;
    act = cdr(act);
    atom = car(act);
  }
  if(!prev)
    return symbol_true;
  return car(prev);
}
cell * not(const cell *operands);