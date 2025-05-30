#include "elfMap.h"
#include <string.h>
#include "print.h"

t_elf_map*          mapFile(const char* path)
{
    t_elf_map* elfMap;

    elfMap = (t_elf_map*)malloc(sizeof(t_elf_map));
    if (!elfMap)
        return NULL;
    ft_bzero(elfMap, sizeof(t_elf_map));
    elfMap->fd = openFile(path, &elfMap->fileSize);
    if (elfMap->fd < 0)
    {
        printNoSuchFile(path);
        free(elfMap);
        return NULL;
    }
    if (elfMap->fileSize <= 1)
    {
        free(elfMap);
        return NULL;
    }
    elfMap->largeFile = elfMap->fileSize > SMALL_FILE_SIZE;
    if (elfMap->fileSize < sizeof(Elf32_Ehdr)
        || mapHeader(elfMap) < 0
        || !MARCH_CALL(elfMap->arch, verifyHeader, elfMap)
        || !MARCH_CALL(elfMap->arch, mapSectionHeaders, elfMap))
    {
        printInvldFileFmt(path);
        unmapFile(elfMap);
        return NULL;
    }
    return elfMap;
}

int mapHeader(t_elf_map* elfMap)
{
    elfMap->fileHdrRange =
        mapFileRangeToMemory(
            elfMap->fd,
            0,
            elfMap->largeFile ? PAGE_4KB : ALIGN_UP(elfMap->fileSize, PAGE_4KB)
        );
    if (!elfMap->fileHdrRange)
        return -1;
    elfMap->elfHeader = (ElfN_Ehdr*)elfMap->fileHdrRange->rangeStart;
    if (ft_memcmp(elfMap->elfHeader->e_ident, ELFMAG, SELFMAG))
    {
        return -1;
    }
    elfMap->arch = elfMap->elfHeader->e_ident[EI_CLASS];
    if (elfMap->arch != ELFCLASS32
        && elfMap->arch != ELFCLASS64)
    {
        return -1;
    }
    return 0;
}

void                unmapFile(t_elf_map* elfMap)
{
    t_list* mappedSection;

    mappedSection = elfMap->mappedSections;
    while (mappedSection)
    {
        unmapFileRange(&((t_mapped_section*)mappedSection->content)->range);
        free(mappedSection->content);
        mappedSection = mappedSection->next;
    }
    ft_lstclear(&elfMap->mappedSections, NULL);
    if (elfMap->fd > 0)
        close(elfMap->fd);
    unmapFileRange(&elfMap->fileHdrRange);
    unmapFileRange(&elfMap->sctnHdrRange);
    free(elfMap);
}
