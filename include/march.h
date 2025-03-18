#ifndef MARCH_H
#define MARCH_H
#include <elf.h>

typedef struct{ 
    unsigned char	e_ident[EI_NIDENT];
}ElfN_Ehdr;

typedef unsigned char* ElfN_Shdr;
typedef struct{ 
    uint32_t      st_name;
}ElfN_Sym;

#define MARCH_CALL(arch, function, ...) \
    ((arch == ELFCLASS32) ? function##32(__VA_ARGS__) : function##64(__VA_ARGS__))

#endif
