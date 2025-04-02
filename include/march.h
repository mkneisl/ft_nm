#ifndef MARCH_H
# define MARCH_H
# include <elf.h>

# ifndef TARGET_ARCH
#  define TARGET_ARCH 32
# endif
# define ELF_VARCH_TYPE(arch, type) Elf##arch##_##type
# define ELF_ARCH_EXPAND(arch, type) ELF_VARCH_TYPE(arch, type)
# define ELF_ARCH_ST_N(arch, t) ELF##arch##_ST_##t
# define ELF_ARCH_ST_EXPAND(arch, t) ELF_ARCH_ST_N(arch, t)
# define ELF_ARCH_ST_TYPE ELF_ARCH_ST_EXPAND(TARGET_ARCH, TYPE)
# define ELF_ARCH_ST_BIND ELF_ARCH_ST_EXPAND(TARGET_ARCH, BIND)

typedef ELF_ARCH_EXPAND(TARGET_ARCH, Ehdr) ElfV_Ehdr;
typedef ELF_ARCH_EXPAND(TARGET_ARCH, Shdr) ElfV_Shdr;
typedef ELF_ARCH_EXPAND(TARGET_ARCH, Sym)  ElfV_Sym;

typedef struct{ 
    unsigned char	e_ident[EI_NIDENT];
}ElfN_Ehdr;

typedef struct{ 
    uint32_t      sh_name;
}ElfN_Shdr;

typedef struct{ 
    uint32_t      st_name;
}ElfN_Sym;

# define MARCH_PAST_FNAME(funcName, arch) funcName##arch
# define MARCH_EXPAND(funcName, arch) MARCH_PAST_FNAME(funcName, arch)
# define MARCH_FUNC(funcName) MARCH_EXPAND(funcName, TARGET_ARCH)

# define MARCH_CALL(arch, function, ...) \
    ((arch == ELFCLASS32) ? function##32(__VA_ARGS__) : function##64(__VA_ARGS__))
# define MARCH_CALL(arch, function, ...) \
    ((arch == ELFCLASS32) ? function##32(__VA_ARGS__) : function##64(__VA_ARGS__))

#endif
