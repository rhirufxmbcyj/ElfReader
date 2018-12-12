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
    switch (elf_header->e_ident[EI_CLASS])
    {
    case ELFCLASSNONE:tmp = transform_string(ELFCLASSNONE); break;
    case ELFCLASS32:tmp = transform_string(ELFCLASS32); break;
    case ELFCLASS64:tmp = transform_string(ELFCLASS64); break;
    case ELFCLASSNUM:tmp = transform_string(ELFCLASSNUM); break;
    }
    tmp += "(" + QString::number(elf_header->e_ident[EI_CLASS]) + ")";
    item->setText(1, tmp);
    item->setText(2, QString::number(offset + EI_CLASS));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_data");
    switch (elf_header->e_ident[EI_DATA])
    {
    case ELFDATANONE:tmp = transform_string(ELFDATANONE); break;
    case ELFDATA2LSB:tmp = transform_string(ELFDATA2LSB); break;
    case ELFDATA2MSB:tmp = transform_string(ELFDATA2MSB); break;
    case ELFDATANUM: tmp = transform_string(ELFDATANUM); break;
    }
    tmp += "(" + QString::number(elf_header->e_ident[EI_DATA]) + ")";
    item->setText(1, tmp);
    item->setText(2, QString::number(offset + EI_DATA));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_version");
    switch (elf_header->e_ident[EI_VERSION])
    {
    case EV_NONE:tmp = transform_string(EV_NONE); break;
    case EV_CURRENT:tmp = transform_string(EV_CURRENT); break;
    case EV_NUM:tmp = transform_string(EV_NUM); break;
    }
    tmp += "(" + QString::number(elf_header->e_ident[EI_VERSION]) + ")";
    item->setText(1, tmp);
    item->setText(2, QString::number(offset + EI_VERSION));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_osabi");
    switch (elf_header->e_ident[EI_OSABI])
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
    tmp += "(" + QString::number(elf_header->e_ident[EI_OSABI]) + ")";
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
    offset += EI_NIDENT;
    switch (elf_header->e_type)
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
    tmp += "(" + QString::number(elf_header->e_type) + ")";
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_type);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_machine");
    offset += size;
    switch (elf_header->e_machine)
    {
    case EM_NONE:tmp = transform_string(EM_NONE);    break;
    case EM_M32:tmp = transform_string(EM_M32);    break;
    case EM_SPARC:tmp = transform_string(EM_SPARC);    break;
    case EM_386:tmp = transform_string(EM_386);    break;
    case EM_68K:tmp = transform_string(EM_68K);    break;
    case EM_88K:tmp = transform_string(EM_88K);    break;
    case EM_IAMCU:tmp = transform_string(EM_IAMCU);    break;
    case EM_860:tmp = transform_string(EM_860);    break;
    case EM_MIPS:tmp = transform_string(EM_MIPS);    break;
    case EM_S370:tmp = transform_string(EM_S370);    break;
    case EM_MIPS_RS3_LE:tmp = transform_string(EM_MIPS_RS3_LE);    break;
    case EM_PARISC:tmp = transform_string(EM_PARISC);    break;
    case EM_VPP500:tmp = transform_string(EM_VPP500);    break;
    case EM_SPARC32PLUS:tmp = transform_string(EM_SPARC32PLUS);    break;
    case EM_960:tmp = transform_string(EM_960);    break;
    case EM_PPC:tmp = transform_string(EM_PPC);    break;
    case EM_PPC64:tmp = transform_string(EM_PPC64);    break;
    case EM_S390:tmp = transform_string(EM_S390);    break;
    case EM_SPU:tmp = transform_string(EM_SPU);    break;
    case EM_V800:tmp = transform_string(EM_V800);    break;
    case EM_FR20:tmp = transform_string(EM_FR20);    break;
    case EM_RH32:tmp = transform_string(EM_RH32);    break;
    case EM_RCE:tmp = transform_string(EM_RCE);    break;
    case EM_ARM:tmp = transform_string(EM_ARM);    break;
    case EM_FAKE_ALPHA:tmp = transform_string(EM_FAKE_ALPHA);    break;
    case EM_SH:tmp = transform_string(EM_SH);    break;
    case EM_SPARCV9:tmp = transform_string(EM_SPARCV9);    break;
    case EM_TRICORE:tmp = transform_string(EM_TRICORE);    break;
    case EM_ARC:tmp = transform_string(EM_ARC);    break;
    case EM_H8_300:tmp = transform_string(EM_H8_300);    break;
    case EM_H8_300H:tmp = transform_string(EM_H8_300H);    break;
    case EM_H8S:tmp = transform_string(EM_H8S);    break;
    case EM_H8_500:tmp = transform_string(EM_H8_500);    break;
    case EM_IA_64:tmp = transform_string(EM_IA_64);    break;
    case EM_MIPS_X:tmp = transform_string(EM_MIPS_X);    break;
    case EM_COLDFIRE:tmp = transform_string(EM_COLDFIRE);    break;
    case EM_68HC12:tmp = transform_string(EM_68HC12);    break;
    case EM_MMA:tmp = transform_string(EM_MMA);    break;
    case EM_PCP:tmp = transform_string(EM_PCP);    break;
    case EM_NCPU:tmp = transform_string(EM_NCPU);    break;
    case EM_NDR1:tmp = transform_string(EM_NDR1);    break;
    case EM_STARCORE:tmp = transform_string(EM_STARCORE);    break;
    case EM_ME16:tmp = transform_string(EM_ME16);    break;
    case EM_ST100:tmp = transform_string(EM_ST100);    break;
    case EM_TINYJ:tmp = transform_string(EM_TINYJ);    break;
    case EM_X86_64:tmp = transform_string(EM_X86_64);    break;
    case EM_PDSP:tmp = transform_string(EM_PDSP);    break;
    case EM_PDP10:tmp = transform_string(EM_PDP10);    break;
    case EM_PDP11:tmp = transform_string(EM_PDP11);    break;
    case EM_FX66:tmp = transform_string(EM_FX66);    break;
    case EM_ST9PLUS:tmp = transform_string(EM_ST9PLUS);    break;
    case EM_ST7:tmp = transform_string(EM_ST7);    break;
    case EM_68HC16:tmp = transform_string(EM_68HC16);    break;
    case EM_68HC11:tmp = transform_string(EM_68HC11);    break;
    case EM_68HC08:tmp = transform_string(EM_68HC08);    break;
    case EM_68HC05:tmp = transform_string(EM_68HC05);    break;
    case EM_SVX:tmp = transform_string(EM_SVX);    break;
    case EM_ST19:tmp = transform_string(EM_ST19);    break;
    case EM_VAX:tmp = transform_string(EM_VAX);    break;
    case EM_CRIS:tmp = transform_string(EM_CRIS);    break;
    case EM_JAVELIN:tmp = transform_string(EM_JAVELIN);    break;
    case EM_FIREPATH:tmp = transform_string(EM_FIREPATH);    break;
    case EM_ZSP:tmp = transform_string(EM_ZSP);    break;
    case EM_MMIX:tmp = transform_string(EM_MMIX);    break;
    case EM_HUANY:tmp = transform_string(EM_HUANY);    break;
    case EM_PRISM:tmp = transform_string(EM_PRISM);    break;
    case EM_AVR:tmp = transform_string(EM_AVR);    break;
    case EM_FR30:tmp = transform_string(EM_FR30);    break;
    case EM_D10V:tmp = transform_string(EM_D10V);    break;
    case EM_D30V:tmp = transform_string(EM_D30V);    break;
    case EM_V850:tmp = transform_string(EM_V850);    break;
    case EM_M32R:tmp = transform_string(EM_M32R);    break;
    case EM_MN10300:tmp = transform_string(EM_MN10300);    break;
    case EM_MN10200:tmp = transform_string(EM_MN10200);    break;
    case EM_PJ:tmp = transform_string(EM_PJ);    break;
    case EM_OPENRISC:tmp = transform_string(EM_OPENRISC);    break;
    case EM_ARC_COMPACT:tmp = transform_string(EM_ARC_COMPACT);    break;
    case EM_XTENSA:tmp = transform_string(EM_XTENSA);    break;
    case EM_VIDEOCORE:tmp = transform_string(EM_VIDEOCORE);    break;
    case EM_TMM_GPP:tmp = transform_string(EM_TMM_GPP);    break;
    case EM_NS32K:tmp = transform_string(EM_NS32K);    break;
    case EM_TPC:tmp = transform_string(EM_TPC);    break;
    case EM_SNP1K:tmp = transform_string(EM_SNP1K);    break;
    case EM_ST200:tmp = transform_string(EM_ST200);    break;
    case EM_IP2K:tmp = transform_string(EM_IP2K);    break;
    case EM_MAX:tmp = transform_string(EM_MAX);    break;
    case EM_CR:tmp = transform_string(EM_CR);    break;
    case EM_F2MC16:tmp = transform_string(EM_F2MC16);    break;
    case EM_MSP430:tmp = transform_string(EM_MSP430);    break;
    case EM_BLACKFIN:tmp = transform_string(EM_BLACKFIN);    break;
    case EM_SE_C33:tmp = transform_string(EM_SE_C33);    break;
    case EM_SEP:tmp = transform_string(EM_SEP);    break;
    case EM_ARCA:tmp = transform_string(EM_ARCA);    break;
    case EM_UNICORE:tmp = transform_string(EM_UNICORE);    break;
    case EM_EXCESS:tmp = transform_string(EM_EXCESS);    break;
    case EM_DXP:tmp = transform_string(EM_DXP);    break;
    case EM_ALTERA_NIOS2:tmp = transform_string(EM_ALTERA_NIOS2);    break;
    case EM_CRX:tmp = transform_string(EM_CRX);    break;
    case EM_XGATE:tmp = transform_string(EM_XGATE);    break;
    case EM_C166:tmp = transform_string(EM_C166);    break;
    case EM_M16C:tmp = transform_string(EM_M16C);    break;
    case EM_DSPIC30F:tmp = transform_string(EM_DSPIC30F);    break;
    case EM_CE:tmp = transform_string(EM_CE);    break;
    case EM_M32C:tmp = transform_string(EM_M32C);    break;
    case EM_TSK3000:tmp = transform_string(EM_TSK3000);    break;
    case EM_RS08:tmp = transform_string(EM_RS08);    break;
    case EM_SHARC:tmp = transform_string(EM_SHARC);    break;
    case EM_ECOG2:tmp = transform_string(EM_ECOG2);    break;
    case EM_SCORE7:tmp = transform_string(EM_SCORE7);    break;
    case EM_DSP24:tmp = transform_string(EM_DSP24);    break;
    case EM_VIDEOCORE3:tmp = transform_string(EM_VIDEOCORE3);    break;
    case EM_LATTICEMICO32:tmp = transform_string(EM_LATTICEMICO32);   break;
    case EM_SE_C17:tmp = transform_string(EM_SE_C17);  break;
    case EM_TI_C6000:tmp = transform_string(EM_TI_C6000);  break;
    case EM_TI_C2000:tmp = transform_string(EM_TI_C2000);  break;
    case EM_TI_C5500:tmp = transform_string(EM_TI_C5500);  break;
    case EM_TI_ARP32:tmp = transform_string(EM_TI_ARP32);  break;
    case EM_TI_PRU:tmp = transform_string(EM_TI_PRU);  break;
    case EM_MMDSP_PLUS:tmp = transform_string(EM_MMDSP_PLUS);  break;
    case EM_CYPRESS_M8C:tmp = transform_string(EM_CYPRESS_M8C);  break;
    case EM_R32C:tmp = transform_string(EM_R32C);  break;
    case EM_TRIMEDIA:tmp = transform_string(EM_TRIMEDIA);  break;
    case EM_QDSP6:tmp = transform_string(EM_QDSP6);  break;
    case EM_8051:tmp = transform_string(EM_8051);  break;
    case EM_STXP7X:tmp = transform_string(EM_STXP7X);  break;
    case EM_NDS32:tmp = transform_string(EM_NDS32);  break;
    case EM_ECOG1X:tmp = transform_string(EM_ECOG1X);  break;
    case EM_MAXQ30:tmp = transform_string(EM_MAXQ30);  break;
    case EM_XIMO16:tmp = transform_string(EM_XIMO16);  break;
    case EM_MANIK:tmp = transform_string(EM_MANIK);  break;
    case EM_CRAYNV2:tmp = transform_string(EM_CRAYNV2);  break;
    case EM_RX:tmp = transform_string(EM_RX);  break;
    case EM_METAG:tmp = transform_string(EM_METAG);  break;
    case EM_MCST_ELBRUS:tmp = transform_string(EM_MCST_ELBRUS);  break;
    case EM_ECOG16:tmp = transform_string(EM_ECOG16);  break;
    case EM_CR16:tmp = transform_string(EM_CR16);  break;
    case EM_ETPU:tmp = transform_string(EM_ETPU);  break;
    case EM_SLE9X:tmp = transform_string(EM_SLE9X);  break;
    case EM_L10M:tmp = transform_string(EM_L10M);  break;
    case EM_K10M:tmp = transform_string(EM_K10M);  break;
    case EM_AARCH64:tmp = transform_string(EM_AARCH64);  break;
    case EM_AVR32:tmp = transform_string(EM_AVR32);  break;
    case EM_STM8:tmp = transform_string(EM_STM8);  break;
    case EM_TILE64:tmp = transform_string(EM_TILE64);  break;
    case EM_TILEPRO:tmp = transform_string(EM_TILEPRO);  break;
    case EM_MICROBLAZE:tmp = transform_string(EM_MICROBLAZE);  break;
    case EM_CUDA:tmp = transform_string(EM_CUDA);  break;
    case EM_TILEGX:tmp = transform_string(EM_TILEGX);  break;
    case EM_CLOUDSHIELD:tmp = transform_string(EM_CLOUDSHIELD);  break;
    case EM_COREA_1ST:tmp = transform_string(EM_COREA_1ST);  break;
    case EM_COREA_2ND:tmp = transform_string(EM_COREA_2ND);  break;
    case EM_ARC_COMPACT2:tmp = transform_string(EM_ARC_COMPACT2);  break;
    case EM_OPEN8:tmp = transform_string(EM_OPEN8);  break;
    case EM_RL78:tmp = transform_string(EM_RL78);  break;
    case EM_VIDEOCORE5:tmp = transform_string(EM_VIDEOCORE5);  break;
    case EM_78KOR:tmp = transform_string(EM_78KOR);  break;
    case EM_56800EX:tmp = transform_string(EM_56800EX);  break;
    case EM_BA1:tmp = transform_string(EM_BA1);  break;
    case EM_BA2:tmp = transform_string(EM_BA2);  break;
    case EM_XCORE:tmp = transform_string(EM_XCORE);  break;
    case EM_MCHP_PIC:tmp = transform_string(EM_MCHP_PIC);  break;
    case EM_KM32:tmp = transform_string(EM_KM32);  break;
    case EM_KMX32:tmp = transform_string(EM_KMX32);  break;
    case EM_EMX16:tmp = transform_string(EM_EMX16);  break;
    case EM_EMX8:tmp = transform_string(EM_EMX8);  break;
    case EM_KVARC:tmp = transform_string(EM_KVARC);  break;
    case EM_CDP:tmp = transform_string(EM_CDP);  break;
    case EM_COGE:tmp = transform_string(EM_COGE);  break;
    case EM_COOL:tmp = transform_string(EM_COOL);  break;
    case EM_NORC:tmp = transform_string(EM_NORC);  break;
    case EM_CSR_KALIMBA:tmp = transform_string(EM_CSR_KALIMBA);  break;
    case EM_Z80:tmp = transform_string(EM_Z80);  break;
    case EM_VISIUM:tmp = transform_string(EM_VISIUM);  break;
    case EM_FT32:tmp = transform_string(EM_FT32);  break;
    case EM_MOXIE:tmp = transform_string(EM_MOXIE);  break;
    case EM_AMDGPU:tmp = transform_string(EM_AMDGPU);  break;
    case EM_RISCV:tmp = transform_string(EM_RISCV);  break;
    case EM_BPF:tmp = transform_string(EM_BPF);  break;
    }
    tmp += "(" + QString::number(elf_header->e_machine) + ")";
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_machine);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_version");
    offset += size;
    switch (elf_header->e_version)
    {
    case EV_NONE:tmp = transform_string(EV_NONE); break;
    case EV_CURRENT:tmp = transform_string(EV_CURRENT); break;
    }
    tmp += "(" + QString::number(elf_header->e_version) + ")";
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_version);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_entry");
    offset += size;
    tmp = "0x" + QString::number(elf_header->e_entry,16);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_entry);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_phoff");
    offset += size;
    tmp = "0x" + QString::number(elf_header->e_phoff,16);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_phoff);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_shoff");
    offset += size;
    tmp = "0x" + QString::number(elf_header->e_shoff,16);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_shoff);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_flags");
    offset += size;
    tmp = QString::number(elf_header->e_flags);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_flags);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_ehsize");
    offset += size;
    tmp = QString::number(elf_header->e_ehsize);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_ehsize);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_phentsize");
    offset += size;
    tmp = QString::number(elf_header->e_phentsize);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_phentsize);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_phnum");
    offset += size;
    tmp = QString::number(elf_header->e_phnum);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_phnum);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_shentsize");
    offset += size;
    tmp = QString::number(elf_header->e_shentsize);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_shentsize);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_shnum");
    offset += size;
    tmp = QString::number(elf_header->e_shnum);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_shnum);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_shstrndx");
    offset += size;
    tmp = QString::number(elf_header->e_shstrndx);
    root->setText(1, tmp);
    root->setText(2, QString::number(offset));
    size = sizeof(elf_header->e_shstrndx);
    root->setText(3, QString::number(size));
    return 0;
}

int ELF_CAT(init_program_header_info)(int offset, void *pointer)
{
    qDebug() << offset;
    ElfReader *pThis = (ElfReader*)pointer;
    Elf_Ehdr *elf_header = (Elf_Ehdr*)pThis->m_data;
    return 0;
}
