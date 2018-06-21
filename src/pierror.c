#include "pierror.h"

/**
 * @brief last error occurred
 *
 */
static int last_error = NO_ERROR;

void pi_error(int CODE, char *message) {
  printf("ERROR: %s\n", message);
  last_error = CODE;
  // free(message);
  longjmp(env_buf, jmp_destination); // jumps to the last saved destination
}

void pi_error_few_args() { pi_error(LISP_ERROR, "too few arguments"); }

void pi_error_many_args() { pi_error(LISP_ERROR, "too many arguments"); }

int get_last_error() { return last_error; }

void reset_error() { last_error = NO_ERROR; }

bool had_error() { return last_error != NO_ERROR; }