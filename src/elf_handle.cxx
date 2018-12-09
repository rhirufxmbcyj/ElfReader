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

int ELF_CAT(init_elf_header_info)(int offset, void *pointer)
{
    ElfReader *pThis = (ElfReader*)pointer;
    Elf_Ehdr *elf_header = (Elf_Ehdr*)pThis->m_data;
    unsigned char *p = (unsigned char *)pThis->m_data;
    QString tmp;
    int size = 0;

    pThis->ui.info_tree->clear();
    QTreeWidgetItem *root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_ident");
    root->setText(2, QString::number(offset));
    root->setText(3, QString::number(EI_NIDENT));

    QTreeWidgetItem *item = new QTreeWidgetItem(root);
    item->setText(0, "identification[4]");
    item->setText(1, ".ELF");
    item->setText(2, QString::number(offset));
    item->setText(3, QString::number(4));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_class");
    switch (*(p + EI_CLASS))
    {
    case ELFCLASSNONE:tmp = transform_string(ELFCLASSNONE); break;
    case ELFCLASS32:tmp = transform_string(ELFCLASS32); break;
    case ELFCLASS64:tmp = transform_string(ELFCLASS64); break;
    case ELFCLASSNUM:tmp = transform_string(ELFCLASSNUM); break;
    }
    tmp += "(" + QString::number(*(p + EI_CLASS)) + ")";
    item->setText(1, tmp);
    item->setText(2, QString::number(offset + EI_CLASS));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_data");
    switch (*(p + EI_DATA))
    {
    case ELFDATANONE:tmp = transform_string(ELFDATANONE); break;
    case ELFDATA2LSB:tmp = transform_string(ELFDATA2LSB); break;
    case ELFDATA2MSB:tmp = transform_string(ELFDATA2MSB); break;
    case ELFDATANUM: tmp = transform_string(ELFDATANUM); break;
    }
    tmp += "(" + QString::number(*(p + EI_DATA)) + ")";
    item->setText(1, tmp);
    item->setText(2, QString::number(offset + EI_DATA));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_version");
    switch (*(p + EI_VERSION))
    {
    case EV_NONE:tmp = transform_string(EV_NONE); break;
    case EV_CURRENT:tmp = transform_string(EV_CURRENT); break;
    case EV_NUM:tmp = transform_string(EV_NUM); break;
    }
    tmp += "(" + QString::number(*(p + EI_VERSION)) + ")";
    item->setText(1, tmp);
    item->setText(2, QString::number(offset + EI_VERSION));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_osabi");
    switch (*(p + EI_OSABI))
    {
    case ELFOSABI_NONE:tmp = transform_string(ELFOSABI_NONE); break;
        //case ELFOSABI_SYSV:tmp = transform_string(ELFOSABI_SYSV); break;
    case ELFOSABI_HPUX:tmp = transform_string(ELFOSABI_HPUX); break;
    case ELFOSABI_NETBSD:tmp = transform_string(ELFOSABI_NETBSD); break;
    case ELFOSABI_GNU:tmp = transform_string(ELFOSABI_GNU); break;
        //case ELFOSABI_LINUX:tmp = transform_string(ELFOSABI_LINUX); break;
    case ELFOSABI_SOLARIS:tmp = transform_string(ELFOSABI_SOLARIS); break;
    case ELFOSABI_AIX:tmp = transform_string(ELFOSABI_AIX); break;
    case ELFOSABI_IRIX:tmp = transform_string(ELFOSABI_IRIX); break;
    case ELFOSABI_FREEBSD:tmp = transform_string(ELFOSABI_FREEBSD); break;
    case ELFOSABI_TRU64:tmp = transform_string(ELFOSABI_TRU64); break;
    case ELFOSABI_MODESTO:tmp = transform_string(ELFOSABI_MODESTO); break;
    case ELFOSABI_OPENBSD:tmp = transform_string(ELFOSABI_OPENBSD); break;
    case ELFOSABI_ARM_AEABI:tmp = transform_string(ELFOSABI_ARM_AEABI); break;
    case ELFOSABI_ARM:tmp = transform_string(ELFOSABI_ARM); break;
    case ELFOSABI_STANDALONE:tmp = transform_string(ELFOSABI_STANDALONE); break;
    }
    tmp += "(" + QString::number(*(p + EI_OSABI)) + ")";
    item->setText(1, tmp);
    item->setText(2, QString::number(offset + EI_OSABI));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_padding[7]");
    item->setText(1, "0");
    item->setText(2, QString::number(offset + EI_PAD));
    item->setText(3, QString::number(7));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_type");
    p += EI_NIDENT;
    offset += EI_NIDENT;
    switch (*p)
    {
    case ET_NONE:tmp = transform_string(ET_NONE); break;
    case ET_REL:tmp = transform_string(ET_REL); break;
    case ET_EXEC:tmp = transform_string(ET_EXEC); break;
    case ET_DYN:tmp = transform_string(ET_DYN); break;
    case ET_CORE:tmp = transform_string(ET_CORE); break;
    case ET_NUM:tmp = transform_string(ET_NUM); break;
    case ET_LOOS:tmp = transform_string(ET_LOOS); break;
    case ET_HIOS:tmp = transform_string(ET_HIOS); break;
    case ET_LOPROC:tmp = transform_string(ET_LOPROC); break;
    case ET_HIPROC:tmp = transform_string(ET_HIPROC); break;
    }
    tmp += "(" + QString::number(*p) + ")";
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(Elf_Half);
    root->setText(3, QString::number(size));

    //     root = new QTreeWidgetItem(ui.info_tree);
    //     root->setText(0, "e_machine");
    //     p += size;
    //     offset += size;
    //     switch(*p)
    //     root->setText(2, QString::number(offset));
    //     root->setText(3, QString::number(size));
    return 0;
}