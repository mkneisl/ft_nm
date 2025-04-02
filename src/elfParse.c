#include "elfMap.h"

int MARCH_FUNC(verifyHeader)(t_elf_map* elfMap)
{
    ElfV_Ehdr* elfHdr;

    elfHdr = (ElfV_Ehdr*)elfMap->fileHdrRange->rangeStart;
    if (elfHdr->e_ident[EI_VERSION] != EV_CURRENT
        || elfHdr->e_ident[EI_DATA] != ELFDATA2LSB) /*little endian (x86_64)*/
    {
        return -1;
    }
    if (elfHdr->e_type != ET_EXEC 
        && elfHdr->e_type != ET_DYN
        && elfHdr->e_type != ET_REL)
    {
        return -1;
    }
    if (elfHdr->e_ehsize != sizeof(ElfV_Ehdr)
        || elfHdr->e_shentsize != sizeof(ElfV_Shdr)
        || elfHdr->e_shoff + elfHdr->e_shentsize * elfHdr->e_shnum > elfMap->fileSize
        || elfHdr->e_shstrndx >= elfHdr->e_shnum)
    {
        return -1;
    }
    return 0;
}

int MARCH_FUNC(mapSectionHeaders)(t_elf_map* elfMap)                         
{                                                                      
    t_mapped_section* mappedSection;                                   
    ElfV_Ehdr* elfHdr;                                          
    size_t size;                                                       

    elfHdr = (ElfV_Ehdr*)elfMap->elfHeader;                     
    size = elfHdr->e_shentsize * elfHdr->e_shnum;                      
    size += elfHdr->e_shoff & (PAGE_4KB - 1);                          
    elfMap->sctnHdrRange =                                             
        mapFileRangeToMemory(                                          
            elfMap->fd,                                                
            ALIGN(elfHdr->e_shoff, PAGE_4KB),                          
            ALIGN_UP(size, PAGE_4KB)                                   
        );                                                             
    if (!elfMap->sctnHdrRange)                                         
        return -1;                                                     
    elfMap->sectionHeader =                                            
        (ElfN_Shdr*)APPLY_OFFSET(                                      
            elfMap->sctnHdrRange,                                      
            elfHdr->e_shoff                                            
        );                                                             
    mappedSection = MARCH_FUNC(mapSection)(                                  
        elfMap,                                                        
        ((ElfV_Shdr*)elfMap->sectionHeader) + elfHdr->e_shstrndx
    );                                                                 
    if (!mappedSection)                                                
        return -1;                                                     
    elfMap->sectionNameTable = (char*)mappedSection->data;             
    return 0;                                                          
}

t_mapped_section* MARCH_FUNC(mapSection)(t_elf_map* elfMap, ElfV_Shdr* section) 
{                                                                                
    size_t size;                                                                 
    t_mapped_section* mappedSection;                                             

    mappedSection = malloc(sizeof(t_mapped_section));                            
    if (!mappedSection)                                                          
        return NULL;                                                             
    ft_bzero(mappedSection, sizeof(t_mapped_section));                           
    size = section->sh_size + (section->sh_offset & (PAGE_4KB - 1));             
    mappedSection->range =                                                       
        mapFileRangeToMemory(                                                    
            elfMap->fd,                                                          
            ALIGN(section->sh_offset, PAGE_4KB),                                 
            ALIGN_UP(size, PAGE_4KB)                                             
        );                                                                       
    if (!mappedSection->range)                                                   
    {                                                                            
        free(mappedSection);                                                     
        return NULL;                                                             
    }                                                                            
    mappedSection->header = (ElfN_Shdr*)section;                                 
    mappedSection->data = APPLY_OFFSET(mappedSection->range, section->sh_offset);
    ft_lstadd_back(&elfMap->mappedSections, ft_lstnew(mappedSection));           
    return mappedSection;                                                        
}


t_mapped_section*   MARCH_FUNC(getSection)(t_elf_map* elfMap, const char* name)                        
{                                                                                                
    size_t nameLen;                                                                              
    t_list* mappedSection;                                                                       
    ElfV_Shdr* sectionHeader;                                                             

    nameLen = ft_strlen(name);                                                                   
    mappedSection = elfMap->mappedSections;                                                      
    while (mappedSection)                                                                        
    {                                                                                            
        sectionHeader = (ElfV_Shdr*)(((t_mapped_section*)mappedSection->content)->header);
        if (!ft_strncmp(                                                                         
            elfMap->sectionNameTable + sectionHeader->sh_name,                                   
            name,                                                                                
            nameLen                                                                              
        ))                                                                                       
            return (t_mapped_section*)mappedSection->content;                                    
        mappedSection = mappedSection->next;                                                     
    }                                                                                            
    for (int i = 0; i < ((ElfV_Ehdr*)elfMap->elfHeader)->e_shnum; i++)                    
    {                                                                                            
        if (!ft_strncmp(                                                                         
            elfMap->sectionNameTable + ((ElfV_Shdr*)elfMap->sectionHeader)[i].sh_name,    
            name,                                                                                
            nameLen                                                                              
        ))                                                                                       
            return MARCH_FUNC(mapSection)(elfMap, ((ElfV_Shdr*)elfMap->sectionHeader) + i);     
    }                                                                                            
    return NULL;                                                                                 
}
