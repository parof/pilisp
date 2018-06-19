/** @defgroup pitestutils
 *
 * @brief Provides tools like prompts to test some functions of pilisp
 *
 */

/** @addtogroup pitestutils */
/*@{*/

#ifndef PTESTUTILS_H
#define PTESTUTILS_H
#include <stdio.h>
#include <setjmp.h>
#include "pilisp.h"

void lexer_prompt();
void parse_prompt();
int lexer_file(FILE * f);
void pairlis_prompt();

#endif // !PTESTUTILS_H

/*}*/