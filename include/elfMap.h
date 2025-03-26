#ifndef FILEMAPPING_H
#define FILEMAPPING_H
#include <stdint.h>
#include <stddef.h>
#include "fileIo.h"
#include "../libft/libft.h"
#include "march.h"

#define PAGE_4KB 0x1000

#define ALIGN(val, align) (((uint64_t)(val)) & ~(align - 1))
#define ALIGN_UP(val, align) (((uint64_t)(val) + (align - 1)) & ~(align - 1))

#define APPLY_OFFSET(range, fileOffset) ((char*)range->rangeStart + (fileOffset - range->offset))

typedef struct s_mapped_section
{
    ElfN_Shdr*      header;
    t_file_range*   range;
    void*           data;
}t_mapped_section;

typedef struct s_elf_map
{
    int             fd;
    size_t          fileSize;
    char            arch;
    ElfN_Ehdr*      elfHeader;
    ElfN_Shdr*      sectionHeader;
    char*           sectionNameTable;
    t_list*         mappedSections;
    t_file_range*   fileHdrRange;
    t_file_range*   sctnHdrRange;
}t_elf_map;

t_elf_map*          mapFile(const char* path);
int                 mapHeader(t_elf_map* elfMap);

int                 verifyHeader32(t_elf_map* elfMap);
int                 verifyHeader64(t_elf_map* elfMap);

int                 mapSectionHeaders64(t_elf_map* elfMap);
int                 mapSectionHeaders32(t_elf_map* elfMap);

t_mapped_section*   mapSection64(t_elf_map* elfMap, Elf64_Shdr* section);
t_mapped_section*   mapSection32(t_elf_map* elfMap, Elf32_Shdr* section);

t_mapped_section*   getSection32(t_elf_map* elfMap, const char* name);
t_mapped_section*   getSection64(t_elf_map* elfMap, const char* name);

void                unmapFile(t_elf_map* elfMap);
#endif
