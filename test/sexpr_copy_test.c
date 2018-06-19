#include "pilisp.h"
#include "pitestutils.h"
#include <stdio.h>

int main(int argc, char **argv) {
  cell *num1 = mk_num(1);
  cell *str1 = mk_str("hi");
  cell *sym1 = mk_sym("var1");
  cell *cons1 = mk_cons(num1, mk_cons(str1,mk_cons(sym1,NULL)));
  print_sexpr(cons1);
  cell *cons2 = copy_cell(cons1);
  print_sexpr(cons2);
  return 0;
}