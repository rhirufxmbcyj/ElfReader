#include "elf_def.h"
#include "ElfReader.h"

void ELF_CAT(parse_elf)(void *pointer)
{
    ElfReader *pThis = (ElfReader*)pointer;
    Elf_Ehdr *elf_header = (Elf_Ehdr*)pThis->m_data;
    pThis->m_info.elf_type = elf_header->e_type;
    pThis->m_info.elf_phoff = elf_header->e_phoff;
    pThis->m_info.elf_phentsize = elf_header->e_phentsize;
    pThis->m_info.elf_phnum = elf_header->e_phnum;
    pThis->m_info.elf_shoff = elf_header->e_shoff;
    pThis->m_info.elf_shentsize = elf_header->e_shentsize;
    pThis->m_info.elf_shnum = elf_header->e_shnum;
    pThis->m_info.elf_shstrndx = elf_header->e_shstrndx;
}