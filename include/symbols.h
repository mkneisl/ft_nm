#ifndef SYMBOLS_H
#define SYMBOLS_H
#include "elfMap.h"

typedef struct s_symbol_data
{
    t_elf_map* elfMap;
    ElfN_Sym** symbols;
    size_t symbolCount;
    char* nameTable;
}t_symbol_data;

t_symbol_data* loadSymbols32(t_elf_map* elfMap);
t_symbol_data* loadSymbols64(t_elf_map* elfMap);

char* getSymbolName(t_symbol_data* symData, ElfN_Sym* symbol);

#endif
