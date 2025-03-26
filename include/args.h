#ifndef ARGS_H
#define ARGS_H
#include "print.h"

typedef struct s_options
{
    char noSort         : 1; // -p
    char reverseSort    : 1; // -r
    char undefinedOnly  : 1; // -u
    char externOnly     : 1; // -g
    char printAll       : 1; // -a
    char help           : 1; // -?
}t_options;

int parseArgs(t_options* options, const char** argv);
int parseFlags(t_options* options, const char*** argv);

#endif
