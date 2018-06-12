#include "ptestutils.h"

void parse_prompt() {
  printf("%sWellcome to the parser prompt, type sexpressions\n", PROMPT_STRING);
  while (1) {
    cell *root = read_sexpr(stdin);
    print_sexpr(root);
  }
}

void lexer_prompt() {
  while (1) {
    printf("%sWellcome to the lexer prompt, type tokens\n", PROMPT_STRING);
    int token =
        next_token(stdin); // note: int, not char, required to handle EOF
    while (1) {
      print_token(token);
      token = next_token(stdin);
    }
  }
  return 0;
}

int lexer_file(FILE *f) {
  while (!feof(f))
    read_sexpr(f);
  return 0;
}