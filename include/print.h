#ifndef PRINT_H
# define PRINT_H
# include "libft.h"
# include "symbols.h"

# define IDENT "nm"


int printInvldOption(char option);
int printHelp();
int printNoSuchFile(const char* file);
int printNoSymbols(const char* file);
int printInvldFileFmt(const char* file);
int printFileTooShort(const char* file);
int printSyms(t_options* options, t_symbol_data* symData);

#endif
