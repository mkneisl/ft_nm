#include "symbols.h"

#define MARCH_loadSymbols(arch)\
t_symbol_data* loadSymbols##arch(t_elf_map* elfMap)\
{\
    t_symbol_data* symData;\
    t_mapped_section* symTab;\
    t_mapped_section* strTab;\
    Elf##arch##_Shdr* symHdr;\
\
    symData = (t_symbol_data*)malloc(sizeof(symData));\
    if (!symData)\
        return NULL;\
    symTab = getSection##arch(elfMap, ".symtab");\
    if (!symTab)\
    {\
        free(symData);\
        return NULL;\
    }\
    symHdr = (Elf##arch##_Shdr*)symTab->header;\
    strTab = getSection##arch(elfMap, ".strtab");\
    if (!strTab)\
    {\
        free(symData);\
        return NULL;\
    }\
    symData->elfMap = elfMap;\
    symData->nameTable = strTab->data;\
    symData->symbolCount = symHdr->sh_size / symHdr->sh_entsize;\
    symData->symbols = (ElfN_Sym**)ft_calloc(symData->symbolCount + 1, sizeof(ElfN_Sym*));\
    if (!symData->symbols)\
    {\
        free(symData);\
        return NULL;\
    }\
    for (size_t i = 0; i < symData->symbolCount; i++)\
        symData->symbols[i] = (ElfN_Sym*)(((Elf##arch##_Sym*)symTab->data) + i);\
    return symData;\
}
MARCH_loadSymbols(32);
MARCH_loadSymbols(64);

char* getSymbolName(t_symbol_data* symData, ElfN_Sym* symbol)
{
    return symData->nameTable + symbol->st_name;
}

