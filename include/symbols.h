#ifndef SYMBOLS_H
# define SYMBOLS_H
# include "elfMap.h"

typedef struct s_symbol_data
{
    t_elf_map* elfMap;
    ElfN_Sym** symbols;
    size_t symbolCount;
    char* nameTable;
}t_symbol_data;

t_symbol_data* loadSymbols32(t_elf_map* elfMap);
t_symbol_data* loadSymbols64(t_elf_map* elfMap);
void clearSymbols(t_symbol_data* symData);

char* getSymbolName64(t_symbol_data* symData, ElfN_Sym* symbol);
char* getSymbolName32(t_symbol_data* symData, ElfN_Sym* symbol);
void sortSymbols(t_symbol_data* symData, int dir);
char getSymbolInfo32(t_symbol_data* symData, ElfN_Sym* symbol, uint64_t* value);
char getSymbolInfo64(t_symbol_data* symData, ElfN_Sym* symbol, uint64_t* value);


#endif
