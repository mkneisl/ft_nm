#include "symbols.h"

bool MARCH_FUNC(loadSymbols)(t_symbol_data* symData, t_mapped_section* symTab)
{
    ElfV_Shdr* symHdr;

    symHdr = (ElfV_Shdr*)symTab->header;
    symData->symbolCount = (symHdr->sh_size / symHdr->sh_entsize) - 1;
    symData->symbols = (ElfN_Sym**)ft_calloc(symData->symbolCount + 1, sizeof(ElfN_Sym*));
    if (!symData->symbols)
        return false;
    for (size_t i = 0; i < symData->symbolCount; i++)
        symData->symbols[i] = (((ElfN_Sym*)symTab->data) + i + 1);
    return true;
}

char* MARCH_FUNC(getSymbolName)(t_symbol_data* symData, ElfN_Sym* symbol)
{
    ElfV_Sym* archSym;

    archSym = (ElfV_Sym*)symbol;
    if (ELF_ARCH_ST_TYPE(archSym->st_info) == STT_SECTION)
        return symData->elfMap->sectionNameTable + ((ElfV_Shdr*)symData->elfMap->sectionHeader)[archSym->st_shndx].sh_name;
    return symData->nameTable + symbol->st_name;
}

char MARCH_FUNC(getSymbolInfo)(t_symbol_data* symData, ElfN_Sym* symbol, uint64_t* value, bool* printVal)
{
    ElfV_Sym* symArch;
    ElfV_Shdr* section;
    size_t bind;
    size_t type;
    
    section = 0;
    symArch = (ElfV_Sym*)symbol;
    *value = symArch->st_value;
    bind = ELF_ARCH_ST_BIND(symArch->st_info);
    type = ELF_ARCH_ST_TYPE(symArch->st_info);
    *printVal = symArch->st_shndx != SHN_UNDEF;
    if (symArch->st_shndx == SHN_UNDEF)
    {
        if (bind != STB_WEAK)
            return 'U';
        if (type == STT_OBJECT)
            return (bind == STB_GLOBAL) ? 'V' : 'v';
        else
            return (bind == STB_GLOBAL) ? 'W' : 'w';
    }
    if (bind == STB_GNU_UNIQUE)
        return 'u';
    if (type == STT_GNU_IFUNC)
        return 'i';
    if (bind == STB_WEAK)
    {
        if (type == STT_OBJECT)
            return 'V';
        else
            return 'W';
    }
    if (symArch->st_shndx == SHN_ABS)
        return (bind == STB_GLOBAL) ? 'A' : 'a';
    if (symArch->st_shndx == SHN_COMMON)
        return (bind == STB_GLOBAL) ? 'C' : 'c';
    section = (ElfV_Shdr*)symData->elfMap->sectionHeader + symArch->st_shndx;
    if (section->sh_type == SHT_NOBITS)
        return (bind == STB_GLOBAL) ? 'B' : 'b';
    if (!ft_strncmp(".sdata", symData->elfMap->sectionNameTable + section->sh_name, 7) 
        || !ft_strncmp(".sbss", symData->elfMap->sectionNameTable + section->sh_name , 6))
        return (bind == STB_GLOBAL) ? 'G' : 'g';
    if (!ft_strncmp(".debug", symData->elfMap->sectionNameTable + section->sh_name, 6))
        return 'N';
    if (section->sh_flags & SHF_ALLOC)
    {
        if (section->sh_flags & SHF_WRITE)
            return (bind == STB_GLOBAL) ? 'D' : 'd';
        else if (section->sh_flags & SHF_EXECINSTR)
            return (bind == STB_GLOBAL) ? 'T' : 't';
        else
            return (bind == STB_GLOBAL) ? 'R' : 'r';
    }
    if (!(section->sh_flags & SHF_WRITE))
        return 'n';
    return '?';
}

bool MARCH_FUNC(isPrintSym)(t_options* options, ElfN_Sym* symbol)
{
    ElfV_Sym* symArch;
    size_t bind;
    size_t type;

    symArch = (ElfV_Sym*)symbol;
    bind = ELF_ARCH_ST_BIND(symArch->st_info);
    type = ELF_ARCH_ST_TYPE(symArch->st_info);
    if (options->undefinedOnly 
        && symArch->st_shndx != SHN_UNDEF)
        return false;
    else if (options->externOnly 
        && (symArch->st_shndx != SHN_COMMON
        && symArch->st_shndx != SHN_UNDEF
        && bind != STB_GLOBAL
        && bind != STB_WEAK))
        return false;
    if (options->pvar)
        return true;
    if (type == STT_SECTION
        || type == STT_FILE)
        return false;
    return true;
}
