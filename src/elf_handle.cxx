#include <assert.h>
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
    pThis->m_info.elf_ehsize = elf_header->e_ehsize;

    //section header string节的offset
    int shstr_off = elf_header->e_shoff + elf_header->e_shstrndx * elf_header->e_shentsize;
    Elf_Shdr *shstr_header = (Elf_Shdr *)(pThis->m_data + shstr_off);
    //拿到section header string的指针
    pThis->m_info.elf_shstr = pThis->m_data + shstr_header->sh_offset;
}

int ELF_CAT(init_elf_header_info)(int offset, void *pointer)
{
    ElfReader *pThis = (ElfReader*)pointer;
    Elf_Ehdr *elf_header = (Elf_Ehdr*)pThis->m_data;
    QTreeWidgetItem *root = NULL;
    QTreeWidgetItem *item = NULL;
    QString tmp;
    int size = 0;

    pThis->ui.info_tree->clear();
    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_ident");
    root->setText(2, "0x" + QString::number(offset,16));
    root->setText(3, QString::number(EI_NIDENT));

    item = new QTreeWidgetItem(root);
    item->setText(0, "identification[4]");
    item->setText(1, ".ELF");
    item->setText(2, "0x" + QString::number(offset,16));
    item->setText(3, QString::number(4));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_class");
    switch (elf_header->e_ident[EI_CLASS])
    {
    case ELFCLASSNONE:tmp = transform_string(ELFCLASSNONE);break;
    case ELFCLASS32:tmp = transform_string(ELFCLASS32);break;
    case ELFCLASS64:tmp = transform_string(ELFCLASS64);break;
    case ELFCLASSNUM:tmp = transform_string(ELFCLASSNUM);break;
    default:assert(0); QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR); break;

    }
    tmp += "(" + QString::number(elf_header->e_ident[EI_CLASS]) + ")";
    item->setText(1, tmp);
    item->setText(2, "0x" + QString::number(offset + EI_CLASS,16));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_data");
    switch (elf_header->e_ident[EI_DATA])
    {
    case ELFDATANONE:tmp = transform_string(ELFDATANONE);break;
    case ELFDATA2LSB:tmp = transform_string(ELFDATA2LSB);break;
    case ELFDATA2MSB:tmp = transform_string(ELFDATA2MSB);break;
    case ELFDATANUM: tmp = transform_string(ELFDATANUM);break;
    default:assert(0); QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR); break;
    }
    tmp += "(" + QString::number(elf_header->e_ident[EI_DATA]) + ")";
    item->setText(1, tmp);
    item->setText(2, "0x" + QString::number(offset + EI_DATA,16));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_version");
    switch (elf_header->e_ident[EI_VERSION])
    {
    case EV_NONE:tmp = transform_string(EV_NONE);break;
    case EV_CURRENT:tmp = transform_string(EV_CURRENT);break;
    case EV_NUM:tmp = transform_string(EV_NUM);break;
    default:assert(0); QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR); break;
    }
    tmp += "(" + QString::number(elf_header->e_ident[EI_VERSION]) + ")";
    item->setText(1, tmp);
    item->setText(2, "0x" + QString::number(offset + EI_VERSION,16));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_osabi");
    switch (elf_header->e_ident[EI_OSABI])
    {
    case ELFOSABI_NONE:tmp = transform_string(ELFOSABI_NONE);break;
        //case ELFOSABI_SYSV:tmp = transform_string(ELFOSABI_SYSV);break;
    case ELFOSABI_HPUX:tmp = transform_string(ELFOSABI_HPUX);break;
    case ELFOSABI_NETBSD:tmp = transform_string(ELFOSABI_NETBSD);break;
    case ELFOSABI_GNU:tmp = transform_string(ELFOSABI_GNU);break;
        //case ELFOSABI_LINUX:tmp = transform_string(ELFOSABI_LINUX);break;
    case ELFOSABI_SOLARIS:tmp = transform_string(ELFOSABI_SOLARIS);break;
    case ELFOSABI_AIX:tmp = transform_string(ELFOSABI_AIX);break;
    case ELFOSABI_IRIX:tmp = transform_string(ELFOSABI_IRIX);break;
    case ELFOSABI_FREEBSD:tmp = transform_string(ELFOSABI_FREEBSD);break;
    case ELFOSABI_TRU64:tmp = transform_string(ELFOSABI_TRU64);break;
    case ELFOSABI_MODESTO:tmp = transform_string(ELFOSABI_MODESTO);break;
    case ELFOSABI_OPENBSD:tmp = transform_string(ELFOSABI_OPENBSD);break;
    case ELFOSABI_ARM_AEABI:tmp = transform_string(ELFOSABI_ARM_AEABI);break;
    case ELFOSABI_ARM:tmp = transform_string(ELFOSABI_ARM);break;
    case ELFOSABI_STANDALONE:tmp = transform_string(ELFOSABI_STANDALONE);break;
    default:assert(0);QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR);break;
    }
    tmp += "(" + QString::number(elf_header->e_ident[EI_OSABI]) + ")";
    item->setText(1, tmp);
    item->setText(2, "0x" + QString::number(offset + EI_OSABI,16));
    item->setText(3, QString::number(1));

    item = new QTreeWidgetItem(root);
    item->setText(0, "ei_padding[7]");
    item->setText(1, "0");
    item->setText(2, "0x" + QString::number(offset + EI_PAD,16));
    item->setText(3, QString::number(7));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_type");
    offset += EI_NIDENT;
    switch (elf_header->e_type)
    {
    case ET_NONE:tmp = transform_string(ET_NONE);break;
    case ET_REL:tmp = transform_string(ET_REL);break;
    case ET_EXEC:tmp = transform_string(ET_EXEC);break;
    case ET_DYN:tmp = transform_string(ET_DYN);break;
    case ET_CORE:tmp = transform_string(ET_CORE);break;
    case ET_NUM:tmp = transform_string(ET_NUM);break;
    case ET_LOOS:tmp = transform_string(ET_LOOS);break;
    case ET_HIOS:tmp = transform_string(ET_HIOS);break;
    case ET_LOPROC:tmp = transform_string(ET_LOPROC);break;
    case ET_HIPROC:tmp = transform_string(ET_HIPROC);break;
    default:assert(0);QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR);break;
    }
    tmp += "(" + QString::number(elf_header->e_type) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_type);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_machine");
    offset += size;
    switch (elf_header->e_machine)
    {
    case EM_NONE:tmp = transform_string(EM_NONE);break;
    case EM_M32:tmp = transform_string(EM_M32);break;
    case EM_SPARC:tmp = transform_string(EM_SPARC);break;
    case EM_386:tmp = transform_string(EM_386);break;
    case EM_68K:tmp = transform_string(EM_68K);break;
    case EM_88K:tmp = transform_string(EM_88K);break;
    case EM_IAMCU:tmp = transform_string(EM_IAMCU);break;
    case EM_860:tmp = transform_string(EM_860);break;
    case EM_MIPS:tmp = transform_string(EM_MIPS);break;
    case EM_S370:tmp = transform_string(EM_S370);break;
    case EM_MIPS_RS3_LE:tmp = transform_string(EM_MIPS_RS3_LE);break;
    case EM_PARISC:tmp = transform_string(EM_PARISC);break;
    case EM_VPP500:tmp = transform_string(EM_VPP500);break;
    case EM_SPARC32PLUS:tmp = transform_string(EM_SPARC32PLUS);break;
    case EM_960:tmp = transform_string(EM_960);break;
    case EM_PPC:tmp = transform_string(EM_PPC);break;
    case EM_PPC64:tmp = transform_string(EM_PPC64);break;
    case EM_S390:tmp = transform_string(EM_S390);break;
    case EM_SPU:tmp = transform_string(EM_SPU);break;
    case EM_V800:tmp = transform_string(EM_V800);break;
    case EM_FR20:tmp = transform_string(EM_FR20);break;
    case EM_RH32:tmp = transform_string(EM_RH32);break;
    case EM_RCE:tmp = transform_string(EM_RCE);break;
    case EM_ARM:tmp = transform_string(EM_ARM);break;
    case EM_FAKE_ALPHA:tmp = transform_string(EM_FAKE_ALPHA);break;
    case EM_SH:tmp = transform_string(EM_SH);break;
    case EM_SPARCV9:tmp = transform_string(EM_SPARCV9);break;
    case EM_TRICORE:tmp = transform_string(EM_TRICORE);break;
    case EM_ARC:tmp = transform_string(EM_ARC);break;
    case EM_H8_300:tmp = transform_string(EM_H8_300);break;
    case EM_H8_300H:tmp = transform_string(EM_H8_300H);break;
    case EM_H8S:tmp = transform_string(EM_H8S);break;
    case EM_H8_500:tmp = transform_string(EM_H8_500);break;
    case EM_IA_64:tmp = transform_string(EM_IA_64);break;
    case EM_MIPS_X:tmp = transform_string(EM_MIPS_X);break;
    case EM_COLDFIRE:tmp = transform_string(EM_COLDFIRE);break;
    case EM_68HC12:tmp = transform_string(EM_68HC12);break;
    case EM_MMA:tmp = transform_string(EM_MMA);break;
    case EM_PCP:tmp = transform_string(EM_PCP);break;
    case EM_NCPU:tmp = transform_string(EM_NCPU);break;
    case EM_NDR1:tmp = transform_string(EM_NDR1);break;
    case EM_STARCORE:tmp = transform_string(EM_STARCORE);break;
    case EM_ME16:tmp = transform_string(EM_ME16);break;
    case EM_ST100:tmp = transform_string(EM_ST100);break;
    case EM_TINYJ:tmp = transform_string(EM_TINYJ);break;
    case EM_X86_64:tmp = transform_string(EM_X86_64);break;
    case EM_PDSP:tmp = transform_string(EM_PDSP);break;
    case EM_PDP10:tmp = transform_string(EM_PDP10);break;
    case EM_PDP11:tmp = transform_string(EM_PDP11);break;
    case EM_FX66:tmp = transform_string(EM_FX66);break;
    case EM_ST9PLUS:tmp = transform_string(EM_ST9PLUS);break;
    case EM_ST7:tmp = transform_string(EM_ST7);break;
    case EM_68HC16:tmp = transform_string(EM_68HC16);break;
    case EM_68HC11:tmp = transform_string(EM_68HC11);break;
    case EM_68HC08:tmp = transform_string(EM_68HC08);break;
    case EM_68HC05:tmp = transform_string(EM_68HC05);break;
    case EM_SVX:tmp = transform_string(EM_SVX);break;
    case EM_ST19:tmp = transform_string(EM_ST19);break;
    case EM_VAX:tmp = transform_string(EM_VAX);break;
    case EM_CRIS:tmp = transform_string(EM_CRIS);break;
    case EM_JAVELIN:tmp = transform_string(EM_JAVELIN);break;
    case EM_FIREPATH:tmp = transform_string(EM_FIREPATH);break;
    case EM_ZSP:tmp = transform_string(EM_ZSP);break;
    case EM_MMIX:tmp = transform_string(EM_MMIX);break;
    case EM_HUANY:tmp = transform_string(EM_HUANY);break;
    case EM_PRISM:tmp = transform_string(EM_PRISM);break;
    case EM_AVR:tmp = transform_string(EM_AVR);break;
    case EM_FR30:tmp = transform_string(EM_FR30);break;
    case EM_D10V:tmp = transform_string(EM_D10V);break;
    case EM_D30V:tmp = transform_string(EM_D30V);break;
    case EM_V850:tmp = transform_string(EM_V850);break;
    case EM_M32R:tmp = transform_string(EM_M32R);break;
    case EM_MN10300:tmp = transform_string(EM_MN10300);break;
    case EM_MN10200:tmp = transform_string(EM_MN10200);break;
    case EM_PJ:tmp = transform_string(EM_PJ);break;
    case EM_OPENRISC:tmp = transform_string(EM_OPENRISC);break;
    case EM_ARC_COMPACT:tmp = transform_string(EM_ARC_COMPACT);break;
    case EM_XTENSA:tmp = transform_string(EM_XTENSA);break;
    case EM_VIDEOCORE:tmp = transform_string(EM_VIDEOCORE);break;
    case EM_TMM_GPP:tmp = transform_string(EM_TMM_GPP);break;
    case EM_NS32K:tmp = transform_string(EM_NS32K);break;
    case EM_TPC:tmp = transform_string(EM_TPC);break;
    case EM_SNP1K:tmp = transform_string(EM_SNP1K);break;
    case EM_ST200:tmp = transform_string(EM_ST200);break;
    case EM_IP2K:tmp = transform_string(EM_IP2K);break;
    case EM_MAX:tmp = transform_string(EM_MAX);break;
    case EM_CR:tmp = transform_string(EM_CR);break;
    case EM_F2MC16:tmp = transform_string(EM_F2MC16);break;
    case EM_MSP430:tmp = transform_string(EM_MSP430);break;
    case EM_BLACKFIN:tmp = transform_string(EM_BLACKFIN);break;
    case EM_SE_C33:tmp = transform_string(EM_SE_C33);break;
    case EM_SEP:tmp = transform_string(EM_SEP);break;
    case EM_ARCA:tmp = transform_string(EM_ARCA);break;
    case EM_UNICORE:tmp = transform_string(EM_UNICORE);break;
    case EM_EXCESS:tmp = transform_string(EM_EXCESS);break;
    case EM_DXP:tmp = transform_string(EM_DXP);break;
    case EM_ALTERA_NIOS2:tmp = transform_string(EM_ALTERA_NIOS2);break;
    case EM_CRX:tmp = transform_string(EM_CRX);break;
    case EM_XGATE:tmp = transform_string(EM_XGATE);break;
    case EM_C166:tmp = transform_string(EM_C166);break;
    case EM_M16C:tmp = transform_string(EM_M16C);break;
    case EM_DSPIC30F:tmp = transform_string(EM_DSPIC30F);break;
    case EM_CE:tmp = transform_string(EM_CE);break;
    case EM_M32C:tmp = transform_string(EM_M32C);break;
    case EM_TSK3000:tmp = transform_string(EM_TSK3000);break;
    case EM_RS08:tmp = transform_string(EM_RS08);break;
    case EM_SHARC:tmp = transform_string(EM_SHARC);break;
    case EM_ECOG2:tmp = transform_string(EM_ECOG2);break;
    case EM_SCORE7:tmp = transform_string(EM_SCORE7);break;
    case EM_DSP24:tmp = transform_string(EM_DSP24);break;
    case EM_VIDEOCORE3:tmp = transform_string(EM_VIDEOCORE3);break;
    case EM_LATTICEMICO32:tmp = transform_string(EM_LATTICEMICO32);break;
    case EM_SE_C17:tmp = transform_string(EM_SE_C17);break;
    case EM_TI_C6000:tmp = transform_string(EM_TI_C6000);break;
    case EM_TI_C2000:tmp = transform_string(EM_TI_C2000);break;
    case EM_TI_C5500:tmp = transform_string(EM_TI_C5500);break;
    case EM_TI_ARP32:tmp = transform_string(EM_TI_ARP32);break;
    case EM_TI_PRU:tmp = transform_string(EM_TI_PRU);break;
    case EM_MMDSP_PLUS:tmp = transform_string(EM_MMDSP_PLUS);break;
    case EM_CYPRESS_M8C:tmp = transform_string(EM_CYPRESS_M8C);break;
    case EM_R32C:tmp = transform_string(EM_R32C);break;
    case EM_TRIMEDIA:tmp = transform_string(EM_TRIMEDIA);break;
    case EM_QDSP6:tmp = transform_string(EM_QDSP6);break;
    case EM_8051:tmp = transform_string(EM_8051);break;
    case EM_STXP7X:tmp = transform_string(EM_STXP7X);break;
    case EM_NDS32:tmp = transform_string(EM_NDS32);break;
    case EM_ECOG1X:tmp = transform_string(EM_ECOG1X);break;
    case EM_MAXQ30:tmp = transform_string(EM_MAXQ30);break;
    case EM_XIMO16:tmp = transform_string(EM_XIMO16);break;
    case EM_MANIK:tmp = transform_string(EM_MANIK);break;
    case EM_CRAYNV2:tmp = transform_string(EM_CRAYNV2);break;
    case EM_RX:tmp = transform_string(EM_RX);break;
    case EM_METAG:tmp = transform_string(EM_METAG);break;
    case EM_MCST_ELBRUS:tmp = transform_string(EM_MCST_ELBRUS);break;
    case EM_ECOG16:tmp = transform_string(EM_ECOG16);break;
    case EM_CR16:tmp = transform_string(EM_CR16);break;
    case EM_ETPU:tmp = transform_string(EM_ETPU);break;
    case EM_SLE9X:tmp = transform_string(EM_SLE9X);break;
    case EM_L10M:tmp = transform_string(EM_L10M);break;
    case EM_K10M:tmp = transform_string(EM_K10M);break;
    case EM_AARCH64:tmp = transform_string(EM_AARCH64);break;
    case EM_AVR32:tmp = transform_string(EM_AVR32);break;
    case EM_STM8:tmp = transform_string(EM_STM8);break;
    case EM_TILE64:tmp = transform_string(EM_TILE64);break;
    case EM_TILEPRO:tmp = transform_string(EM_TILEPRO);break;
    case EM_MICROBLAZE:tmp = transform_string(EM_MICROBLAZE);break;
    case EM_CUDA:tmp = transform_string(EM_CUDA);break;
    case EM_TILEGX:tmp = transform_string(EM_TILEGX);break;
    case EM_CLOUDSHIELD:tmp = transform_string(EM_CLOUDSHIELD);break;
    case EM_COREA_1ST:tmp = transform_string(EM_COREA_1ST);break;
    case EM_COREA_2ND:tmp = transform_string(EM_COREA_2ND);break;
    case EM_ARC_COMPACT2:tmp = transform_string(EM_ARC_COMPACT2);break;
    case EM_OPEN8:tmp = transform_string(EM_OPEN8);break;
    case EM_RL78:tmp = transform_string(EM_RL78);break;
    case EM_VIDEOCORE5:tmp = transform_string(EM_VIDEOCORE5);break;
    case EM_78KOR:tmp = transform_string(EM_78KOR);break;
    case EM_56800EX:tmp = transform_string(EM_56800EX);break;
    case EM_BA1:tmp = transform_string(EM_BA1);break;
    case EM_BA2:tmp = transform_string(EM_BA2);break;
    case EM_XCORE:tmp = transform_string(EM_XCORE);break;
    case EM_MCHP_PIC:tmp = transform_string(EM_MCHP_PIC);break;
    case EM_KM32:tmp = transform_string(EM_KM32);break;
    case EM_KMX32:tmp = transform_string(EM_KMX32);break;
    case EM_EMX16:tmp = transform_string(EM_EMX16);break;
    case EM_EMX8:tmp = transform_string(EM_EMX8);break;
    case EM_KVARC:tmp = transform_string(EM_KVARC);break;
    case EM_CDP:tmp = transform_string(EM_CDP);break;
    case EM_COGE:tmp = transform_string(EM_COGE);break;
    case EM_COOL:tmp = transform_string(EM_COOL);break;
    case EM_NORC:tmp = transform_string(EM_NORC);break;
    case EM_CSR_KALIMBA:tmp = transform_string(EM_CSR_KALIMBA);break;
    case EM_Z80:tmp = transform_string(EM_Z80);break;
    case EM_VISIUM:tmp = transform_string(EM_VISIUM);break;
    case EM_FT32:tmp = transform_string(EM_FT32);break;
    case EM_MOXIE:tmp = transform_string(EM_MOXIE);break;
    case EM_AMDGPU:tmp = transform_string(EM_AMDGPU);break;
    case EM_RISCV:tmp = transform_string(EM_RISCV);break;
    case EM_BPF:tmp = transform_string(EM_BPF);break;
    default:assert(0);QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR);break;
    }
    tmp += "(" + QString::number(elf_header->e_machine) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_machine);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_version");
    offset += size;
    switch (elf_header->e_version)
    {
    case EV_NONE:tmp = transform_string(EV_NONE);break;
    case EV_CURRENT:tmp = transform_string(EV_CURRENT);break;
    default:assert(0);QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR);break;
    }
    tmp += "(" + QString::number(elf_header->e_version) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_version);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_entry");
    offset += size;
    tmp = "0x" + QString::number(elf_header->e_entry,16);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_entry);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_phoff");
    offset += size;
    tmp = "0x" + QString::number(elf_header->e_phoff,16);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_phoff);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_shoff");
    offset += size;
    tmp = "0x" + QString::number(elf_header->e_shoff,16);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_shoff);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_flags");
    offset += size;
    tmp = QString::number(elf_header->e_flags);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_flags);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_ehsize");
    offset += size;
    tmp = QString::number(elf_header->e_ehsize);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_ehsize);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_phentsize");
    offset += size;
    tmp = QString::number(elf_header->e_phentsize);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_phentsize);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_phnum");
    offset += size;
    tmp = QString::number(elf_header->e_phnum);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_phnum);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_shentsize");
    offset += size;
    tmp = QString::number(elf_header->e_shentsize);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_shentsize);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_shnum");
    offset += size;
    tmp = QString::number(elf_header->e_shnum);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_shnum);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "e_shstrndx");
    offset += size;
    tmp = QString::number(elf_header->e_shstrndx);
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(elf_header->e_shstrndx);
    root->setText(3, QString::number(size));
    return 0;
}

int ELF_CAT(init_program_header_info)(int offset, void *pointer)
{
    ElfReader *pThis = (ElfReader*)pointer;
    Elf_Phdr *program_header = (Elf_Phdr*)(pThis->m_data + offset);
    QTreeWidgetItem *root = NULL;
    QString tmp;
    int size = 0;

    pThis->ui.info_tree->clear();
    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "p_type");
    switch (program_header->p_type)
    {
    case PT_NULL:tmp = transform_string(PT_NULL);break;
    case PT_LOAD:tmp = transform_string(PT_LOAD);break;
    case PT_DYNAMIC:tmp = transform_string(PT_DYNAMIC);break;
    case PT_INTERP:tmp = transform_string(PT_INTERP);break;
    case PT_NOTE:tmp = transform_string(PT_NOTE);break;
    case PT_SHLIB:tmp = transform_string(PT_SHLIB);break;
    case PT_PHDR:tmp = transform_string(PT_PHDR);break;
    case PT_TLS:tmp = transform_string(PT_TLS);break;
    case PT_NUM:tmp = transform_string(PT_NUM);break;
    case PT_LOOS:tmp = transform_string(PT_LOOS);break;
    case PT_GNU_EH_FRAME:tmp = transform_string(PT_GNU_EH_FRAME);break;
    case PT_GNU_STACK:tmp = transform_string(PT_GNU_STACK);break;
    case PT_GNU_RELRO:tmp = transform_string(PT_GNU_RELRO);break;
    //case PT_LOSUNW:tmp = transform_string(PT_LOSUNW);break;
    case PT_SUNWBSS:tmp = transform_string(PT_SUNWBSS);break;
    case PT_SUNWSTACK:tmp = transform_string(PT_SUNWSTACK);break;
    //case PT_HISUNW:tmp = transform_string(PT_HISUNW);break;
    case PT_HIOS:tmp = transform_string(PT_HIOS);break;
    case PT_LOPROC:tmp = transform_string(PT_LOPROC);break;
    case PT_HIPROC:tmp = transform_string(PT_HIPROC);break;
    default:assert(0);QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR);break;
    }
    tmp += "(0x" + QString::number(program_header->p_type,16) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(program_header->p_type);
    root->setText(3, QString::number(size));

#ifdef ELF64
    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "p_flags");
    switch (program_header->p_flags)
    {
    case PF_X:tmp = "PF_Exec"; break;
    case PF_W:tmp = "PF_Write"; break;
    case PF_R:tmp = "PF_Read"; break;
    case PF_R | PF_W:tmp = "PF_Read_Write"; break;
    case PF_R | PF_X:tmp = "PF_Read_Exec"; break;
    case PF_W | PF_X:tmp = "PF_Write_Exec"; break;
    case PF_R | PF_W | PF_X:tmp = "PF_Read_Write_Exec"; break;
    default:assert(0);QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR);break;
    }
    tmp += "(" + QString::number(program_header->p_flags) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(program_header->p_flags);
    root->setText(3, QString::number(size));
#endif // ELF64

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0,"p_offset");
    root->setText(1, "0x" + QString::number(program_header->p_offset,16));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(program_header->p_offset);
    root->setText(3, QString::number(size));
    
    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "p_vaddr");
    root->setText(1, "0x" + QString::number(program_header->p_vaddr, 16));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(program_header->p_vaddr);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "p_paddr");
    root->setText(1, "0x" + QString::number(program_header->p_paddr, 16));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(program_header->p_paddr);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "p_filesz");
    root->setText(1, "0x" + QString::number(program_header->p_filesz, 16));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(program_header->p_filesz);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "p_memsz");
    root->setText(1, "0x" + QString::number(program_header->p_memsz, 16));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(program_header->p_memsz);
    root->setText(3, QString::number(size));

#ifdef ELF32
    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "p_flags");
    switch (program_header->p_flags)
    {
    case PF_X:tmp = "PF_Exec"; break;
    case PF_W:tmp = "PF_Write"; break;
    case PF_R:tmp = "PF_Read"; break;
    case PF_R | PF_W:tmp = "PF_Read_Write"; break;
    case PF_R | PF_X:tmp = "PF_Read_Exec"; break;
    case PF_W | PF_X:tmp = "PF_Write_Exec"; break;
    case PF_R | PF_W | PF_X:tmp = "PF_Read_Write_Exec"; break;
    default:assert(0);QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR);break;
    }
    tmp += "(" + QString::number(program_header->p_flags) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(program_header->p_flags);
    root->setText(3, QString::number(size));
#endif // ELF32

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "p_align");
    root->setText(1, "0x" + QString::number(program_header->p_align, 16));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(program_header->p_align);
    root->setText(3, QString::number(size));
    return 0;
}

int ELF_CAT(init_section_header_info)(int offset, void *pointer)
{
    ElfReader *pThis = (ElfReader*)pointer;
    Elf_Shdr *section_header = (Elf_Shdr*)(pThis->m_data + offset);
    QTreeWidgetItem *root = NULL;
    QString tmp;
    int size = 0;

    pThis->ui.info_tree->clear();
    root = new QTreeWidgetItem(pThis->ui.info_tree);
    root->setText(0, "sh_name");
    tmp = pThis->m_info.elf_shstr + section_header->sh_name;
    tmp += "(0x" + QString::number(section_header->sh_name, 16) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(section_header->sh_name);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_type");
    switch (section_header->sh_type)
    {
    case SHT_NULL:tmp = transform_string(SHT_NULL); break;
    case SHT_PROGBITS:tmp = transform_string(SHT_PROGBITS); break;
    case SHT_SYMTAB:tmp = transform_string(SHT_SYMTAB); break;
    case SHT_STRTAB:tmp = transform_string(SHT_STRTAB); break;
    case SHT_RELA:tmp = transform_string(SHT_RELA); break;
    case SHT_HASH:tmp = transform_string(SHT_HASH); break;
    case SHT_DYNAMIC:tmp = transform_string(SHT_DYNAMIC); break;
    case SHT_NOTE:tmp = transform_string(SHT_NOTE); break;
    case SHT_NOBITS:tmp = transform_string(SHT_NOBITS); break;
    case SHT_REL:tmp = transform_string(SHT_REL); break;
    case SHT_SHLIB:tmp = transform_string(SHT_SHLIB); break;
    case SHT_DYNSYM:tmp = transform_string(SHT_DYNSYM); break;
    case SHT_INIT_ARRAY:tmp = transform_string(SHT_INIT_ARRAY); break;
    case SHT_FINI_ARRAY:tmp = transform_string(SHT_FINI_ARRAY); break;
    case SHT_PREINIT_ARRAY:tmp = transform_string(SHT_PREINIT_ARRAY); break;
    case SHT_GROUP:tmp = transform_string(SHT_GROUP); break;
    case SHT_SYMTAB_SHNDX:tmp = transform_string(SHT_SYMTAB_SHNDX); break;
    case SHT_NUM:tmp = transform_string(SHT_NUM); break;
    case SHT_LOOS:tmp = transform_string(SHT_LOOS); break;
    case SHT_GNU_ATTRIBUTES:tmp = transform_string(SHT_GNU_ATTRIBUTES); break;
    case SHT_GNU_HASH:tmp = transform_string(SHT_GNU_HASH); break;
    case SHT_GNU_LIBLIST:tmp = transform_string(SHT_GNU_LIBLIST); break;
    case SHT_CHECKSUM:tmp = transform_string(SHT_CHECKSUM); break;
    case SHT_LOSUNW:tmp = transform_string(SHT_LOSUNW); break;
    //case SHT_SUNW_move:tmp = transform_string(SHT_SUNW_move); break;
    case SHT_SUNW_COMDAT:tmp = transform_string(SHT_SUNW_COMDAT); break;
    case SHT_SUNW_syminfo:tmp = transform_string(SHT_SUNW_syminfo); break;
    case SHT_GNU_verdef:tmp = transform_string(SHT_GNU_verdef); break;
    case SHT_GNU_verneed:tmp = transform_string(SHT_GNU_verneed); break;
    case SHT_GNU_versym:tmp = transform_string(SHT_GNU_versym); break;
        //case SHT_HISUNW:tmp = transform_string(SHT_HISUNW); break;
        //case SHT_HIOS:tmp = transform_string(SHT_HIOS); break;
    case SHT_LOPROC:tmp = transform_string(SHT_LOPROC); break;
    case SHT_HIPROC:tmp = transform_string(SHT_HIPROC); break;
    case SHT_LOUSER:tmp = transform_string(SHT_LOUSER); break;
    case SHT_HIUSER:tmp = transform_string(SHT_HIUSER); break;
    default:assert(0);QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR);break;
    }
    tmp += "(0x" + QString::number(section_header->sh_type, 16) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(section_header->sh_type);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_flags");
    tmp.clear();
    if (section_header->sh_flags & SHF_WRITE)
        tmp += "Write ";
    if (section_header->sh_flags & SHF_ALLOC)
        tmp += "Alloc";
    if (section_header->sh_flags & SHF_EXECINSTR)
        tmp += "Exec ";
    if (section_header->sh_flags & SHF_MERGE)
        tmp += "Merge ";
    if (section_header->sh_flags & SHF_STRINGS)
        tmp += "String ";
    if (section_header->sh_flags & SHF_INFO_LINK)
        tmp += "Info_Link ";
    if (section_header->sh_flags & SHF_LINK_ORDER)
        tmp += "Link Order ";
    if (section_header->sh_flags & SHF_OS_NONCONFORMING)
        tmp += "OS_NONCONFORMING ";
    if (section_header->sh_flags & SHF_GROUP)
        tmp += "GROUP ";
    if (section_header->sh_flags & SHF_TLS)
        tmp += "TLS ";
    if (section_header->sh_flags & SHF_COMPRESSED)
        tmp += "COMPRESSED ";
    if (section_header->sh_flags & SHF_ORDERED)
        tmp += "ORDERED ";
    if (section_header->sh_flags & SHF_EXCLUDE)
        tmp += "EXCLUDE ";
    tmp += "(0x" + QString::number(section_header->sh_flags, 16) + ")";
    root->setText(1, tmp);
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(section_header->sh_flags);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_addr");
    root->setText(1, "0x" + QString::number(section_header->sh_addr, 16));
    root->setText(2, "0x" + QString::number(offset,16));
    size = sizeof(section_header->sh_addr);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_offset");
    root->setText(1, "0x" + QString::number(section_header->sh_offset, 16));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(section_header->sh_offset);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_size");
    root->setText(1, "0x" + QString::number(section_header->sh_size, 16));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(section_header->sh_size);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_link");
    root->setText(1, QString::number(section_header->sh_link));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(section_header->sh_link);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_info");
    root->setText(1, QString::number(section_header->sh_info));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(section_header->sh_info);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_addralign");
    root->setText(1, QString::number(section_header->sh_addralign));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(section_header->sh_addralign);
    root->setText(3, QString::number(size));

    root = new QTreeWidgetItem(pThis->ui.info_tree);
    offset += size;
    root->setText(0, "sh_entsize");
    root->setText(1, QString::number(section_header->sh_entsize));
    root->setText(2, "0x" + QString::number(offset, 16));
    size = sizeof(section_header->sh_entsize);
    root->setText(3, QString::number(size));
    return 0;
}

int ELF_CAT(init_dynamic_section)(int offset, void *pointer)
{
    ElfReader *pThis = (ElfReader*)pointer;
    QTreeWidgetItem *root = NULL;
    QString tmp;
    Elf_Dyn *dyn = (Elf_Dyn *)(pThis->m_data + offset);
    int i = 0;
    pThis->ui.info_tree->clear();
    while (dyn->d_tag != DT_NULL)
    {
        root = new QTreeWidgetItem(pThis->ui.info_tree);
        root->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_DYNAMIC_INFO);
        root->setData(0, ITEM_DATA_ITEM_OFFSET, offset + i * sizeof(Elf_Dyn));
        root->setData(0, ITEM_DATA_ITEM_SIZE, sizeof(Elf_Dyn));
        root->setText(0, "0x" + QString::number(dyn->d_tag, 16));
        switch (dyn->d_tag)
        {
        case DT_NEEDED:        tmp = transform_string(DT_NEEDED); break;
        case DT_PLTRELSZ:      tmp = transform_string(DT_PLTRELSZ); break;
        case DT_PLTGOT:        tmp = transform_string(DT_PLTGOT); break;
        case DT_HASH:          tmp = transform_string(DT_HASH); break;
        case DT_STRTAB:        tmp = transform_string(DT_STRTAB); break;
        case DT_SYMTAB:        tmp = transform_string(DT_SYMTAB); break;
        case DT_RELA:          tmp = transform_string(DT_RELA); break;
        case DT_RELASZ:        tmp = transform_string(DT_RELASZ); break;
        case DT_RELAENT:       tmp = transform_string(DT_RELAENT); break;
        case DT_STRSZ:         tmp = transform_string(DT_STRSZ); break;
        case DT_SYMENT:        tmp = transform_string(DT_SYMENT); break;
        case DT_INIT:          tmp = transform_string(DT_INIT); break;
        case DT_FINI:          tmp = transform_string(DT_FINI); break;
        case DT_SONAME:        tmp = transform_string(DT_SONAME); break;
        case DT_RPATH:         tmp = transform_string(DT_RPATH); break;
        case DT_SYMBOLIC:      tmp = transform_string(DT_SYMBOLIC); break;
        case DT_REL:           tmp = transform_string(DT_REL); break;
        case DT_RELSZ:         tmp = transform_string(DT_RELSZ); break;
        case DT_RELENT:        tmp = transform_string(DT_RELENT); break;
        case DT_PLTREL:        tmp = transform_string(DT_PLTREL); break;
        case DT_DEBUG:         tmp = transform_string(DT_DEBUG); break;
        case DT_TEXTREL:       tmp = transform_string(DT_TEXTREL); break;
        case DT_JMPREL:        tmp = transform_string(DT_JMPREL); break;
        case DT_BIND_NOW:      tmp = transform_string(DT_BIND_NOW); break;
        case DT_INIT_ARRAY:    tmp = transform_string(DT_INIT_ARRAY); break;
        case DT_FINI_ARRAY:    tmp = transform_string(DT_FINI_ARRAY); break;
        case DT_INIT_ARRAYSZ:  tmp = transform_string(DT_INIT_ARRAYSZ); break;
        case DT_FINI_ARRAYSZ:  tmp = transform_string(DT_FINI_ARRAYSZ); break;
        case DT_RUNPATH:       tmp = transform_string(DT_RUNPATH); break;
        case DT_FLAGS:         tmp = transform_string(DT_FLAGS); break;
        case DT_ENCODING:      tmp = transform_string(DT_ENCODING); break;
        case DT_PREINIT_ARRAYSZ:tmp = transform_string(DT_PREINIT_ARRAYSZ); break;
        case DT_SYMTAB_SHNDX:  tmp = transform_string(DT_SYMTAB_SHNDX); break;
        case DT_NUM:           tmp = transform_string(DT_NUM); break;
        case DT_LOOS:          tmp = transform_string(DT_LOOS); break;
        case DT_HIOS:          tmp = transform_string(DT_HIOS); break;
        case DT_LOPROC:        tmp = transform_string(DT_LOPROC); break;
        case DT_HIPROC:        tmp = transform_string(DT_HIPROC); break;
        case DT_VALRNGLO:      tmp = transform_string(DT_VALRNGLO); break;
        case DT_GNU_PRELINKED: tmp = transform_string(DT_GNU_PRELINKED); break;
        case DT_GNU_CONFLICTSZ:tmp = transform_string(DT_GNU_CONFLICTSZ); break;
        case DT_GNU_LIBLISTSZ: tmp = transform_string(DT_GNU_LIBLISTSZ); break;
        case DT_CHECKSUM:      tmp = transform_string(DT_CHECKSUM); break;
        case DT_PLTPADSZ:      tmp = transform_string(DT_PLTPADSZ); break;
        case DT_MOVEENT:       tmp = transform_string(DT_MOVEENT); break;
        case DT_MOVESZ:        tmp = transform_string(DT_MOVESZ); break;
        case DT_FEATURE_1:      tmp = transform_string(DT_FEATURE_1); break;
        case DT_POSFLAG_1:      tmp = transform_string(DT_POSFLAG_1); break;
        case DT_SYMINSZ:       tmp = transform_string(DT_SYMINSZ); break;
        case DT_SYMINENT:      tmp = transform_string(DT_SYMINENT); break;
        case DT_ADDRRNGLO:     tmp = transform_string(DT_ADDRRNGLO); break;
        case DT_GNU_HASH:      tmp = transform_string(DT_GNU_HASH); break;
        case DT_TLSDESC_PLT:   tmp = transform_string(DT_TLSDESC_PLT); break;
        case DT_TLSDESC_GOT:   tmp = transform_string(DT_TLSDESC_GOT); break;
        case DT_GNU_CONFLICT:  tmp = transform_string(DT_GNU_CONFLICT); break;
        case DT_GNU_LIBLIST:   tmp = transform_string(DT_GNU_LIBLIST); break;
        case DT_CONFIG:        tmp = transform_string(DT_CONFIG); break;
        case DT_DEPAUDIT:      tmp = transform_string(DT_DEPAUDIT); break;
        case DT_AUDIT:         tmp = transform_string(DT_AUDIT); break;
        case DT_PLTPAD:        tmp = transform_string(DT_PLTPAD); break;
        case DT_MOVETAB:       tmp = transform_string(DT_MOVETAB); break;
        case DT_SYMINFO:       tmp = transform_string(DT_SYMINFO); break;
        case DT_VERSYM:        tmp = transform_string(DT_VERSYM); break;
        case DT_RELACOUNT:     tmp = transform_string(DT_RELACOUNT); break;
        case DT_RELCOUNT:      tmp = transform_string(DT_RELCOUNT); break;
        case DT_VERDEF:        tmp = transform_string(DT_VERDEF); break;
        case DT_VERDEFNUM:     tmp = transform_string(DT_VERDEFNUM); break;
        case DT_VERNEED:       tmp = transform_string(DT_VERNEED); break;
        case DT_VERNEEDNUM:    tmp = transform_string(DT_VERNEEDNUM); break;
        default:assert(0); QMessageBox::information(pThis, MESSAGE_CAPTION, MESSAGE_TYPE_ERROR); break;
        }
        root->setText(1, tmp);
        if (dyn->d_tag == DT_NEEDED)
        {
            char *lib_name = pThis->m_info.elf_dynstr + dyn->d_un.d_val;
            root->setText(2, lib_name);
        }
        else
        {
            root->setText(2, "0x" + QString::number(dyn->d_un.d_val, 16));
        }
        dyn++;
        i++;
    }
    return 0;
}