#include "piinit.h"

void init_builtin_lambdas() {
  builtin_lambdas_index = 0;
  symbol_car = mk_builtin_lambda("CAR");
  symbol_cdr = mk_builtin_lambda("CDR");
  symbol_cons = mk_builtin_lambda("CONS");
  symbol_atom = mk_builtin_lambda("ATOM");
  symbol_eq = mk_builtin_lambda("EQ");
  symbol_eq_math = mk_builtin_lambda("=");
  symbol_true = mk_builtin_lambda("T");
  symbol_addition = mk_builtin_lambda("+");
  symbol_subtraction = mk_builtin_lambda("-");
  symbol_multiplication = mk_builtin_lambda("*");
  symbol_division = mk_builtin_lambda("/");
  symbol_set = mk_builtin_lambda("SET");
  symbol_lambda = mk_builtin_lambda("LAMBDA");
  symbol_label = mk_builtin_lambda("LABEL");
  symbol_quote = mk_builtin_lambda("QUOTE");
  symbol_cond = mk_builtin_lambda("COND");
  symbol_load = mk_builtin_lambda("LOAD");
  symbol_or = mk_builtin_lambda("OR");
  symbol_and = mk_builtin_lambda("AND");
  symbol_not = mk_builtin_lambda("NOT");
  symbol_greater = mk_builtin_lambda(">");
  symbol_greater_equal = mk_builtin_lambda(">=");
  symbol_less = mk_builtin_lambda("<");
  symbol_less_equal = mk_builtin_lambda("<=");
  symbol_length = mk_builtin_lambda("LENGTH");
  symbol_member = mk_builtin_lambda("MEMBER");
  symbol_nth = mk_builtin_lambda("NTH");
  symbol_file_ended = mk_builtin_lambda("FEOF");
  symbol_env = mk_builtin_lambda("ENV");
  symbol_mem_dump = mk_builtin_lambda("MD");
  symbol_collect_garbage = mk_builtin_lambda("CG");
  symbol_dotimes = mk_builtin_lambda("DOTIMES");
  symbol_list = mk_builtin_lambda("LIST");
  symbol_bye = mk_builtin_lambda("BYE");
  symbol_macro = mk_builtin_lambda("MACRO");
  symbol_integerp = mk_builtin_lambda("INTEGERP");
  symbol_symbolp = mk_builtin_lambda("SYMBOLP");
  symbol_write = mk_builtin_lambda("WRITE");
  symbol_subseq = mk_builtin_lambda("SUBSEQ");
  symbol_reverse = mk_builtin_lambda("REVERSE");
}

void init_builtin_macros() {
  builtin_macros_index = 0;
  symbol_setq = mk_builtin_macro("SETQ");
  symbol_let = mk_builtin_macro("LET");
  symbol_defun = mk_builtin_macro("DEFUN");
  symbol_timer = mk_builtin_macro("TIME");
  symbol_map = mk_builtin_macro("MAP");
}

void init_env() {
  memory->global_env = NULL;
  // write the basic functions to one file, then load them
  write_program_to_file(
      ".piinit",
      "(set 'defmacro (macro (name param body) " // check this
      "(list 'set (list 'quote name) (list 'macro param body))))"

      "(defun 1+ (num) (+ num 1))"

      "(setq d \"./test/lisp_programs/diff.lisp\")"

      "(setq p \"./examples/a.lisp\")"

      "(setq f \"./examples/functions.lisp\")");
  parse_file(".piinit");
  cell_space_destroy_stack(memory); // remove thrash
  collect_garbage(memory);
}

void init_pi() {
  init_builtin_macros();
  init_builtin_lambdas();
  init_memory();
  init_env();
}

void free_pi() {
  free_memory();
  free_builtin_symbols();
}

void free_builtin_symbols() {
  size_t i = 0;
  for (i = 0; i < builtin_lambdas_index; i++)
    free_cell_pointed_memory(&BUILTIN_LAMBDAS[i]);
  for (i = 0; i < builtin_macros_index; i++)
    free_cell_pointed_memory(&BUILTIN_MACROS[i]);
}