#include "symbols.h"

t_symbol_data* initSymbols(t_elf_map* elfMap)
{
    t_mapped_section* symTab;
    t_mapped_section* strTab;
    t_symbol_data* symData;

    symData = (t_symbol_data*)malloc(sizeof(t_symbol_data));
    if (!symData)
        return NULL;
    symTab = MARCH_CALL(elfMap->arch, getSection, elfMap, ".symtab");
    if (!symTab)
    {
        free(symData);
        return NULL;
    }
    strTab = MARCH_CALL(elfMap->arch, getSection, elfMap, ".strtab");
    if (!strTab)
    {
        free(symData);
        return NULL;
    }
    symData->elfMap = elfMap;
    symData->nameTable = strTab->data;
    if (!MARCH_CALL(elfMap->arch, loadSymbols, symData, symTab))
    {
        free(symData);
        return NULL;
    }
    return symData;
}

void clearSymbols(t_symbol_data* symData)
{
    free(symData->symbols);
    free(symData);
}

void sortSymbols(t_symbol_data* symData, int dir)
{
    char* symName;
    ElfN_Sym* symbol;
    size_t symNameLen;
    int cmpResult;

    for (size_t i = 1; i < symData->symbolCount; i++)
    {
        symbol = symData->symbols[i];
        symName = MARCH_CALL(symData->elfMap->arch, getSymbolName, symData, symbol);
        symNameLen = ft_strlen(symName) + 1;
        int j = i;
        while  (--j >= 0)
        {
            cmpResult = ft_strncmp(
                symName,
                MARCH_CALL(symData->elfMap->arch, getSymbolName, symData, symData->symbols[j]),
                symNameLen
            );
            if ((dir < 0) ? cmpResult <= 0 : cmpResult >= 0)
                break;
            symData->symbols[j + 1] = symData->symbols[j];
        }
        symData->symbols[j + 1] = symbol;
    }
}
