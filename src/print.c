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
    ft_printf("  -a,                    Display debugger-only symbols\n");
    ft_printf("  -a,                    Display debugger-only symbols\n");
    ft_printf("  -a,                    Display debugger-only symbols\n");
    ft_printf("  -a,                    Display debugger-only symbols\n");
    return 0;
}

int printNoSuchFile(const char* file)
{
    ft_printf(IDENT": '%s': No such file\n", file);
    return -1;
}

int printNoSymbols(const char* file)
{
    ft_printf(IDENT": '%s': no symbols\n", file);
    return -1;
}

int printInvldFileFmt(const char* file)
{
    ft_printf(IDENT": '%s': file format not recognized\n", file);
    return -1;
}
int printFileTooShort(const char* file)
{
    ft_printf(IDENT": '%s': file too short\n", file);
    return -1;
}