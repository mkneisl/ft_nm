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
    if (elfMap->fileSize < 20 
        || mapHeader(elfMap) < 0 
        || MARCH_CALL(elfMap->arch, verifyHeader, elfMap) < 0
        || MARCH_CALL(elfMap->arch, mapSectionHeaders, elfMap) < 0)
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
            PAGE_4KB
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

#define MARCH_verifyHeader(arch)\
int verifyHeader##arch(t_elf_map* elfMap) \
{\
    Elf##arch##_Ehdr* elfHdr;\
\
    elfHdr = (Elf##arch##_Ehdr*)elfMap->fileHdrRange->rangeStart;\
    if (elfHdr->e_ident[EI_VERSION] != EV_CURRENT\
        || elfHdr->e_ident[EI_DATA] != ELFDATA2LSB) /*little endian (x86_64)*/\
    {\
        return -1;\
    }\
    if (elfHdr->e_type != ET_EXEC \
        && elfHdr->e_type != ET_DYN\
        && elfHdr->e_type != ET_REL)\
    {\
        return -1;\
    }\
    if (elfHdr->e_ehsize != sizeof(Elf##arch##_Ehdr)\
        || elfHdr->e_shentsize != sizeof(Elf##arch##_Shdr)\
        || elfHdr->e_shoff + elfHdr->e_shentsize * elfHdr->e_shnum > elfMap->fileSize\
        || elfHdr->e_shstrndx >= elfHdr->e_shnum)\
    {\
        return -1;\
    }\
    return 0;\
}
MARCH_verifyHeader(32)
MARCH_verifyHeader(64)

#define MARCH_mapSectionHeaders(arch)\
int mapSectionHeaders##arch(t_elf_map* elfMap)                         \
{                                                                      \
    t_mapped_section* mappedSection;                                   \
    Elf##arch##_Ehdr* elfHdr;                                          \
    size_t size;                                                       \
\
    elfHdr = (Elf##arch##_Ehdr*)elfMap->elfHeader;                     \
    size = elfHdr->e_shentsize * elfHdr->e_shnum;                      \
    size += elfHdr->e_shoff & (PAGE_4KB - 1);                          \
    elfMap->sctnHdrRange =                                             \
        mapFileRangeToMemory(                                          \
            elfMap->fd,                                                \
            ALIGN(elfHdr->e_shoff, PAGE_4KB),                          \
            ALIGN_UP(size, PAGE_4KB)                                   \
        );                                                             \
    if (!elfMap->sctnHdrRange)                                         \
        return -1;                                                     \
    elfMap->sectionHeader =                                            \
        (ElfN_Shdr*)APPLY_OFFSET(                                      \
            elfMap->sctnHdrRange,                                      \
            elfHdr->e_shoff                                            \
        );                                                             \
    mappedSection = mapSection##arch(                                  \
        elfMap,                                                        \
        ((Elf##arch##_Shdr*)elfMap->sectionHeader) + elfHdr->e_shstrndx\
    );                                                                 \
    if (!mappedSection)                                                \
        return -1;                                                     \
    elfMap->sectionNameTable = (char*)mappedSection->data;             \
    return 0;                                                          \
}
MARCH_mapSectionHeaders(32)
MARCH_mapSectionHeaders(64)

#define MARCH_mapSection(arch)\
t_mapped_section* mapSection##arch(t_elf_map* elfMap, Elf##arch##_Shdr* section) \
{                                                                                \
    size_t size;                                                                 \
    t_mapped_section* mappedSection;                                             \
\
    mappedSection = malloc(sizeof(t_mapped_section));                            \
    if (!mappedSection)                                                          \
        return NULL;                                                             \
    ft_bzero(mappedSection, sizeof(t_mapped_section));                           \
    size = section->sh_size + (section->sh_offset & (PAGE_4KB - 1));             \
    mappedSection->range =                                                       \
        mapFileRangeToMemory(                                                    \
            elfMap->fd,                                                          \
            ALIGN(section->sh_offset, PAGE_4KB),                                 \
            ALIGN_UP(size, PAGE_4KB)                                             \
        );                                                                       \
    if (!mappedSection->range)                                                   \
    {                                                                            \
        free(mappedSection);                                                     \
        return NULL;                                                             \
    }                                                                            \
    mappedSection->header = (ElfN_Shdr*)section;                                 \
    mappedSection->data = APPLY_OFFSET(mappedSection->range, section->sh_offset);\
    ft_lstadd_back(&elfMap->mappedSections, ft_lstnew(mappedSection));           \
    return mappedSection;                                                        \
}
MARCH_mapSection(32)
MARCH_mapSection(64)

#define MARCH_getSection(arch)\
t_mapped_section*   getSection##arch(t_elf_map* elfMap, const char* name)                        \
{                                                                                                \
    size_t nameLen;                                                                              \
    t_list* mappedSection;                                                                       \
    Elf##arch##_Shdr* sectionHeader;                                                             \
\
    nameLen = ft_strlen(name);                                                                   \
    mappedSection = elfMap->mappedSections;                                                      \
    while (mappedSection)                                                                        \
    {                                                                                            \
        sectionHeader = (Elf##arch##_Shdr*)(((t_mapped_section*)mappedSection->content)->header);\
        if (!ft_strncmp(                                                                         \
            elfMap->sectionNameTable + sectionHeader->sh_name,                                   \
            name,                                                                                \
            nameLen                                                                              \
        ))                                                                                       \
            return (t_mapped_section*)mappedSection->content;                                    \
        mappedSection = mappedSection->next;                                                     \
    }                                                                                            \
    for (int i = 0; i < ((Elf##arch##_Ehdr*)elfMap->elfHeader)->e_shnum; i++)                    \
    {                                                                                            \
        if (!ft_strncmp(                                                                         \
            elfMap->sectionNameTable + ((Elf##arch##_Shdr*)elfMap->sectionHeader)[i].sh_name,    \
            name,                                                                                \
            nameLen                                                                              \
        ))                                                                                       \
            return mapSection##arch(elfMap, ((Elf##arch##_Shdr*)elfMap->sectionHeader) + i);     \
    }                                                                                            \
    return NULL;                                                                                 \
}
MARCH_getSection(32)
MARCH_getSection(64)

void                unmapFile(t_elf_map* elfMap)
{
    t_list* mappedSection;  
    
    mappedSection = elfMap->mappedSections;
    while (mappedSection)                                                                         
    {                  
        unmapFileRange(&((t_mapped_section*)mappedSection->content)->range);
        mappedSection = mappedSection->next;                                                      
    }
    ft_lstclear(&elfMap->mappedSections, NULL);
    if (elfMap->fd > 0)
        close(elfMap->fd);
    unmapFileRange(&elfMap->fileHdrRange);                                          
    unmapFileRange(&elfMap->sctnHdrRange);            
    free(elfMap);                              
}
