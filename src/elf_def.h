#ifndef ELF_DEF_H
#define ELF_DEF_H

#include "elf.h"



#ifdef  ELF32
typedef    Elf32_Ehdr    Elf_Ehdr;
#define    ELF_CAT(_name_)    elf32_##_name_

#endif


#ifdef  ELF64
typedef    Elf64_Ehdr    Elf_Ehdr;
#define    ELF_CAT(_name_)    elf64_##_name_

#endif


void elf32_parse_elf(void *pointer);
void elf64_parse_elf(void *pointer);

#endif // !ELF_DEF_H
