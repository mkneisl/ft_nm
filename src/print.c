#include "print.h"

int printInvldOption(char option)
{
    ft_printf(IDENT": invalid option -- '%c'\n", option);
    return -1;
}

int printHelp()
{
    ft_printf("Usage: "IDENT" [option(s)] [file(s)]\n List symbols in [file(s)] (a.out by default).\n");
    ft_printf(" The options are:\n");
    ft_printf("  -a,                    Display debugger-only symbols\n");
    ft_printf("  -g,                    Display only external symbols\n");
    ft_printf("  -r,                    Reverse the sense of the sort\n");
    ft_printf("  -u,                    Display only undefined symbols\n");
    ft_printf("  -p,                    Do not sort the symbols\n");
    ft_printf("  -h,                    Display this information\n");
    return 0;
}

int printNoSuchFile(const char* file)
{
    ft_printf(IDENT": '%s': No such file\n", file);
    return -1;
}

int printNoSymbols(const char* file)
{
    ft_printf(IDENT": %s: no symbols\n", file);
    return -1;
}

int printInvldFileFmt(const char* file)
{
    ft_printf(IDENT": %s: file format not recognized\n", file);
    return -1;
}

int printFileTooShort(const char* file)
{
    ft_printf(IDENT": '%s': file too short\n", file);
    return -1;
}

int printSyms(t_options* options, t_symbol_data* symData)
{
    for (size_t i = 0; i < symData->symbolCount; i++){
        uint64_t symVal = 0;
        bool printVal;
        char* symName = MARCH_CALL(symData->elfMap->arch, getSymbolName, symData, symData->symbols[i]);
        int symChar = MARCH_CALL(symData->elfMap->arch, getSymbolInfo, symData, symData->symbols[i], &symVal, &printVal);

        if (!MARCH_CALL(symData->elfMap->arch, isPrintSym, options, symData->symbols[i]))
            continue;
        if (symData->elfMap->arch == ELFCLASS32)
        {
            if (printVal)
                ft_printf("%08lx %c %s\n", symVal, symChar, symName);
            else
                ft_printf("%8s %c %s\n", "", symChar, symName);
        }
        else
        {
            if (printVal)
                ft_printf("%016lx %c %s\n", symVal, symChar, symName);
            else
                ft_printf("%16s %c %s\n", "", symChar, symName);
        }
    }
    return 0;
}