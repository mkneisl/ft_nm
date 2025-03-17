#ifndef SYMBOLS_H
#define SYMBOLS_H
#include "elfMap.h"

typedef struct s_symbol_data
{
    t_elf_map* elfMap;
    ElfN_Sym** symbols;
    char* nameTable;
}t_symbol_data;

int loadSymbols32(t_elf_map* elfMap, int sorting);
int loadSymbols64(t_elf_map* elfMap, int sorting);

int getSymbolType64(ElfN_Sym* symbol);
int getSymbolType32(ElfN_Sym* symbol);
#endif
