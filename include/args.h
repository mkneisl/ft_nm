#ifndef ARGS_H
# define ARGS_H
# include <stdint.h> 

typedef struct s_options
{
    union {
        struct{
            uint8_t help           : 1; // -?
            uint8_t noSort         : 1; // -p
            uint8_t sortDir        : 1; // -r
            uint8_t undefinedOnly  : 1; // -u
            uint8_t externOnly     : 1; // -g
            uint8_t printAll       : 1; // -a
            uint8_t pad1           : 2;
        };
        struct{
            uint8_t pad2           : 3;
            uint8_t pvar           : 3;
            uint8_t pad3           : 2;
        };
    };
}t_options;

int parseArgs(t_options* options, const char** argv);
int parseFlags(t_options* options, const char*** argv);

#endif
