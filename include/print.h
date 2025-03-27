#ifndef PRINT_H
# define PRINT_H
# include "libft.h"

# define IDENT "ft_nm"

int printInvldOption(char option);
int printHelp();
int printNoSuchFile(const char* file);
int printNoSymbols(const char* file);
int printInvldFileFmt(const char* file);
int printFileTooShort(const char* file);

#endif
