#include "main.h"
#include "print.h"


int nmFile(t_options* options, const char* elfFile)
{
    t_elf_map* elfMap;
    t_symbol_data* symData;

    elfMap = mapFile(elfFile);
    if (!elfMap)
        return -1;
    symData = MARCH_CALL(elfMap->arch, loadSymbols, elfMap);
    if (!symData)
    {
        unmapFile(elfMap);
        return printNoSymbols(elfFile);
    }
    if (!options->noSort)
        sortSymbols(symData, (int)options->sortDir * -1);
    printSyms(options, symData);
    unmapFile(elfMap);
    return 0;
}

int main(int argc, const char** argv)
{
    int invldFiles;
    char openDefault;
    t_options nmOption;

    if (parseArgs(&nmOption, argv) < 0)
    {
        printHelp();
        return 1;
    }
    if (nmOption.help)
        return printHelp();
    invldFiles = 0;
    openDefault = 1;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
            continue;
        openDefault = 0;
        if (nmFile(&nmOption, argv[i]) < 0)
            invldFiles++;
    }
    if (!openDefault)
        return invldFiles;
    if (nmFile(&nmOption, "a.out") < 0)
        return 1;
    return 0;
}
