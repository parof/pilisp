#include "pierror.h"

/**
 * @brief last error occurred
 * 
 */
static int last_error=NO_ERROR;

void pi_error(int CODE, const char* message){
    printf("%s ERROR: %s\n",PROMPT_STRING,message);
    last_error = CODE;
    longjmp(env_buf,jmp_destination); // jumps to the last saved destination
}

void pi_error_args(){
    pi_error(LISP_ERROR,"too few arguments");
}

int get_last_error(){
    return last_error;
}

void reset_error(){
    last_error = NO_ERROR;
}

bool had_error(){
    return last_error != NO_ERROR;
}