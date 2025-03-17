#include "main.h"

int main(int argc, char** argv)
{
    char* fileName;
    int symbolIdx;
    char* symStrTable;
    t_elf_map* elfMap;
    t_mapped_section* symTab;
    t_mapped_section* strTab;
    ElfN_Sym* symbols;

    if (argc < 2)
        fileName = "a.out";
    else
        fileName = argv[1];
    elfMap = mapFile(fileName);
    if (!elfMap)
    {
        printf("Cant map file\n");
        return 1;
    }
    symTab = MARCH_CALL(elfMap->arch, getSection, elfMap, ".symtab");
    if (!symTab)
    {
        printf("No symbols\n");
        return 1;
    }
    strTab = MARCH_CALL(elfMap->arch, getSection, elfMap, ".strtab");
    if (!strTab)
    {
        printf("No symbol names\n");
        return 1;
    }
    symStrTable = (char*)strTab->data;
    symbols = (ElfN_Sym*)symTab->data;
   
    unmapFile(elfMap);
    return 0;
}
