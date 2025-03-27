#include "symbols.h"

void clearSymbols(t_symbol_data* symData)
{
    free(symData->symbols);
    free(symData);
}

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
    symData->symbolCount = (symHdr->sh_size / symHdr->sh_entsize) - 1;\
    symData->symbols = (ElfN_Sym**)ft_calloc(symData->symbolCount + 1, sizeof(ElfN_Sym*));\
    if (!symData->symbols)\
    {\
        free(symData);\
        return NULL;\
    }\
    for (size_t i = 0; i < symData->symbolCount; i++)\
        symData->symbols[i] = (ElfN_Sym*)(((Elf##arch##_Sym*)symTab->data) + i + 1);\
    return symData;\
}
MARCH_loadSymbols(32);
MARCH_loadSymbols(64);

#define MARCH_getSymbolName(arch)\
char* getSymbolName##arch(t_symbol_data* symData, ElfN_Sym* symbol)\
{\
    Elf##arch##_Sym* archSym;\
\
    archSym = (Elf##arch##_Sym*)symbol;\
    if (ELF64_ST_TYPE(archSym->st_info) == STT_SECTION)\
        return symData->elfMap->sectionNameTable + ((Elf##arch##_Shdr*)symData->elfMap->sectionHeader)[archSym->st_shndx].sh_name;\
    return symData->nameTable + symbol->st_name;\
}
MARCH_getSymbolName(32)
MARCH_getSymbolName(64)

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
        symNameLen = ft_strlen(symName);
        int j = i;
        while  (--j >= 0)
        {
            cmpResult = ft_strncmp(
                symName,
                MARCH_CALL(symData->elfMap->arch, getSymbolName, symData, symData->symbols[j]),
                symNameLen
            );
            if ((dir < 0) ? cmpResult <= 0 : cmpResult > 0)
                break;
            symData->symbols[j + 1] = symData->symbols[j];
        }
        symData->symbols[j + 1] = symbol;
    }
}

#define MARCH_getSymbolInfo(arch)\
char getSymbolInfo##arch(t_symbol_data* symData, ElfN_Sym* symbol, uint64_t* value)\
{\
    Elf##arch##_Sym* symArch;\
    Elf##arch##_Shdr* section;\
    size_t bind;\
    size_t type;\
    \
    section = 0;\
    symArch = (Elf##arch##_Sym*)symbol;\
    *value = symArch->st_value;\
    bind = ELF##arch##_ST_BIND(symArch->st_info);\
    type = ELF##arch##_ST_TYPE(symArch->st_info);\
    if (symArch->st_shndx == SHN_UNDEF)\
    {\
        if (bind != STB_WEAK)\
            return 'U';\
        if (type == STT_OBJECT)\
            return (bind == STB_GLOBAL) ? 'V' : 'v';\
        else\
            return (bind == STB_GLOBAL) ? 'W' : 'w';\
    }\
    if (bind == STB_GNU_UNIQUE)\
        return 'u';\
    if (type == STT_GNU_IFUNC)\
        return 'i';\
    if (symArch->st_shndx == SHN_ABS)\
        return (bind == STB_GLOBAL) ? 'A' : 'a';\
    if (symArch->st_shndx == SHN_COMMON)\
        return (bind == STB_GLOBAL) ? 'C' : 'c';\
    section = (Elf##arch##_Shdr*)symData->elfMap->sectionHeader + symArch->st_shndx;\
    if (section->sh_type == SHT_NOBITS)\
        return (bind == STB_GLOBAL) ? 'B' : 'b';\
    if (!ft_strncmp(".sdata", symData->elfMap->sectionNameTable + section->sh_name, 7) \
        || !ft_strncmp(".sbss", symData->elfMap->sectionNameTable + section->sh_name , 6))\
        return (bind == STB_GLOBAL) ? 'G' : 'g';\
    if (!ft_strncmp(".debug", symData->elfMap->sectionNameTable + section->sh_name, 6))\
        return 'N';\
    if (section->sh_flags & SHF_ALLOC)\
    {\
        if (section->sh_flags & SHF_WRITE)\
            return (bind == STB_GLOBAL) ? 'D' : 'd';\
        else if (section->sh_flags & SHF_EXECINSTR)\
            return (bind == STB_GLOBAL) ? 'T' : 't';\
        else\
            return (bind == STB_GLOBAL) ? 'R' : 'r';\
    }\
    if (!(section->sh_flags & SHF_WRITE))\
        return 'n';\
    return '?';\
}
MARCH_getSymbolInfo(32)
MARCH_getSymbolInfo(64)