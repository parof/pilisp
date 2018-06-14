/** @defgroup pierror
 *
 * @brief Provides errors handling
 *
 */

/** @addtogroup pierror */
/*@{*/
#ifndef PERROR_H
#define PERROR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilisp.h"

/**
 * @brief enum to idenfity the type of the last erro
 * 
 */
enum{
    NO_ERROR = -1,           // @brief no error occurrred
    LISP_ERROR = 1,          // @brief LISP syntax error
    MEMORY_ERROR = 2,        // @brief memory error
    MODE_ERROR = 3           // @brief error passing mode to some kind of functions
};

/**
 * @brief last error occurred
 * 
 */
static int last_error=NO_ERROR;

/**
 * @brief sets the last error to CODE, and sets one message. It will longjump to the last saved point
 * 
 * @param CODE code of the error
 * @param message message to display
 */
void pi_error(int CODE, const char* message);

/**
 * @brief Get the last error code
 * 
 * @return int last error code
 */
int get_last_error();

/**
 * @brief one error just verificated?
 * 
 * @return true yes
 * @return false no
 */
bool had_error();

/**
 * @brief after reading one error this function should be called to indicate that the error has benn handled
 * 
 */
void reset_error();

#endif // !PERROR_H
/*@}*/