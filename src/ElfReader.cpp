#include "ElfReader.h"
#include <assert.h>
#include "elf_def.h"

ElfReader::ElfReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    init_info_header(0);
    //设置splitter比例 左边占2份 右边占5份
    ui.splitter_left->setStretchFactor(0, 2);
    ui.splitter_left->setStretchFactor(1, 5);
    ui.splitter_right->setStretchFactor(0, 1);
    ui.splitter_right->setStretchFactor(1, 1);
    //连接analyze信号槽
    QObject::connect(this, SIGNAL(start_analyze(QString)), this, SLOT(start_analyze_slot(QString)));
    m_data = NULL;
    hexEdit = new QHexEdit(ui.hex);
    //hexEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void ElfReader::dropEvent(QDropEvent *event)
{
    QString file_name = event->mimeData()->urls().first().toLocalFile();
    //qDebug() << file_name;
    emit start_analyze(file_name);
}

void ElfReader::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void ElfReader::paintEvent(QPaintEvent *event)
{
    if(hexEdit->size() != ui.hex->size())
        hexEdit->setFixedSize(ui.hex->size());
}

void ElfReader::pushButton_open_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, u8"请选择一个ELF文件", "", "", 0);
    //qDebug() << file_name;
    emit start_analyze(file_name);
}

void ElfReader::pushButton_save_clicked()
{
    qDebug() << transform_string(save_file);
}

void ElfReader::start_analyze_slot(QString file_name)
{
    //qDebug() << file_name;
    if (file_name == "")
        return;
    QFile file(file_name);
    QByteArray file_data;
    clean_up_data();
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, MESSAGE_CAPTION, MESSAGE_OPEN_FILE_ERROR);
        goto End;
    }
    file_data = file.readAll();
    file.close();
    m_data = new char[file_data.size()];
    memcpy(m_data, file_data.data(), file_data.size());
    //检查文件合法性
    if (check_file_format() != ELF_SUCCESS)
    {
        QMessageBox::warning(ui.centralWidget, MESSAGE_CAPTION, MESSAGE_FILE_FORMAT_ERROR);
        goto End;
    }
    QObject::connect(ui.header_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(header_item_changed_slot(QTreeWidgetItem*, QTreeWidgetItem*)));
    QObject::connect(ui.info_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(info_item_changed_slot(QTreeWidgetItem*, QTreeWidgetItem*)));
    hexEdit->setData(file_data);
    parse_elf();
    init_header_tree();
End:
    return;
}

void ElfReader::header_item_changed_slot(QTreeWidgetItem *current_item, QTreeWidgetItem *pre_item)
{
    if (current_item == NULL)
        return;
    int header_type = current_item->data(0, ITEM_DATA_ITEM_TYPE).toInt();
    qint64 offset = current_item->data(0, ITEM_DATA_ITEM_OFFSET).toLongLong();
    qint64 size = current_item->data(0, ITEM_DATA_ITEM_SIZE).toLongLong();
    init_info_header(0);
    switch (header_type)
    {
    case ITEM_DATA_ELF_HEADER:
        if (m_info.elf_is_x64)
            elf64_init_elf_header_info(offset, this);
        else
            elf32_init_elf_header_info(offset, this);
        break;
    case ITEM_DATA_ELF_PROGRAM_HEADER:
        //不处理
        break;
    case ITEM_DATA_ELF_PROGRAM_ITEM:
        if (m_info.elf_is_x64)
            elf64_init_program_header_info(offset, this);
        else
            elf32_init_program_header_info(offset, this);
        break;
    case ITEM_DATA_ELF_SECTION_HEADER:
    case ITEM_DATA_ELF_SYMTAB_HEADER:
        //不处理
        break;
    case ITEM_DATA_ELF_SECTION_ITEM:
        if (m_info.elf_is_x64)
            elf64_init_section_header_info(offset, this);
        else
            elf32_init_section_header_info(offset, this);
        break;
    case ITEM_DATA_ELF_DYNAMIC_HEADER:
        init_info_header(1);
        if (m_info.elf_is_x64)
            elf64_init_dynamic_section(offset, this);
        else
            elf32_init_dynamic_section(offset, this);
        break;
    case ITEM_DATA_ELF_SYMTAB_ITEM:
        if (m_info.elf_is_x64)
            elf64_init_symtab_header_info(offset, this);
        else
            elf32_init_symtab_header_info(offset, this);
        break;
    default:assert(0); break;
    }
    hexEdit->JumpSelect(offset, size);
}

void ElfReader::info_item_changed_slot(QTreeWidgetItem *current_item, QTreeWidgetItem *pre_item)
{
    if (current_item == NULL)
        return;
    qint64 offset = 0, size = 0;
    if (current_item->data(0, ITEM_DATA_ITEM_TYPE).toInt() == ITEM_DATA_ELF_DYNAMIC_INFO)
    {
        offset = current_item->data(0, ITEM_DATA_ITEM_OFFSET).toLongLong();
        size = current_item->data(0, ITEM_DATA_ITEM_SIZE).toLongLong();
    }
    else
    {
        offset = current_item->text(2).toLongLong(nullptr, 16);
        size = current_item->text(3).toLongLong();
    }
    hexEdit->JumpSelect(offset, size);
}

void ElfReader::init_info_header(int type)
{
    QStringList info_header;
    if (type == 0)
    {
        ui.info_tree->setColumnCount(4);
        info_header << "    Name" << "Value" << "Start" << "Size";
        ui.info_tree->setHeaderLabels(info_header);
    }
    else
    {
        ui.info_tree->setColumnCount(3);
        info_header << "    Tag" << "Name" << "Value"; 
        ui.info_tree->setHeaderLabels(info_header);
    }
}

void ElfReader::clean_up_data()
{
    if (m_data)
    {
        delete m_data;
        m_data = NULL;
    }
    //清空list tree控件
    QObject::disconnect(ui.header_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(header_item_changed_slot(QTreeWidgetItem*, QTreeWidgetItem*)));
    ui.header_tree->clear();
    ui.info_tree->clear();
    memset(&m_info, 0, sizeof(elf_info_st));
}

int ElfReader::check_file_format()
{
    assert(m_data);
    if (*((unsigned int*)m_data) != *((unsigned int*)ELFMAG))
        return ELF_INVALID_FORMAT;
    return ELF_SUCCESS;
}

void ElfReader::parse_elf()
{
    if (m_data == NULL)
        return;
    m_info.elf_is_x64 = m_data[EI_CLASS] == (char)ELFCLASS64 ? TRUE : FALSE;
    if (m_info.elf_is_x64)
    {
        elf64_parse_elf(this);
    }
    else
    {
        elf32_parse_elf(this);
    }
}

int ElfReader::init_header_tree()
{
    int ret;
    ret = init_elf_header();
    if (ret != ELF_SUCCESS)
        goto End;
    ret = init_program_header();
    if (ret != ELF_SUCCESS)
        goto End;
    ret = init_section_header();
    if (ret != ELF_SUCCESS)
        goto End;
    ret = init_dynamic_section();
    if (ret != ELF_SUCCESS)
        goto End;
    ret = init_symbol_section();
    if (ret != ELF_SUCCESS)
        goto End;
    ret = init_dynsym_section();
    if (ret != ELF_SUCCESS)
        goto End;
End:
    return ret;
}

int ElfReader::init_elf_header()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
    QString elf_header = "Elf Header";
    if (m_info.elf_is_x64 == TRUE)
        elf_header += "(x64)";
    else
        elf_header += "(x86)";
    root->setText(0,elf_header);
    root->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_HEADER);
    root->setData(0, ITEM_DATA_ITEM_OFFSET, 0);
    root->setData(0, ITEM_DATA_ITEM_SIZE, m_info.elf_ehsize);
    ui.header_tree->setCurrentItem(root);
    return ELF_SUCCESS;
}

int ElfReader::init_program_header()
{
    if (m_info.elf_phnum == 0)
        return ELF_SUCCESS;
    QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
    QString tmp;
    root->setText(0, "Program Header Table");
    root->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_PROGRAM_HEADER);
    root->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_phoff);
    root->setData(0, ITEM_DATA_ITEM_SIZE, m_info.elf_phentsize * m_info.elf_phnum);
    for (int i = 0; i < m_info.elf_phnum; i++)
    {
        tmp.sprintf("Program Header Table[%d]", i);
        QTreeWidgetItem *item = new QTreeWidgetItem(root);
        item->setText(0,tmp);
        item->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_PROGRAM_ITEM);
        item->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_phoff + i * m_info.elf_phentsize);
        item->setData(0, ITEM_DATA_ITEM_SIZE, m_info.elf_phentsize);
    }
    return ELF_SUCCESS;
}

int ElfReader::init_section_header()
{
    if (m_info.elf_shnum == 0)
        return ELF_SUCCESS;
    QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
    QString sec_name;
    QString tmp;
    root->setText(0, "Section Header Table");
    root->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_SECTION_HEADER);
    root->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_shoff);
    root->setData(0, ITEM_DATA_ITEM_SIZE, m_info.elf_shentsize * m_info.elf_shnum);
    for (int i = 0; i < m_info.elf_shnum; i++)
    {
        //要加上section名
        Elf64_Shdr *sec_hdr_64 = (Elf64_Shdr*)(m_data + m_info.elf_shoff + i * m_info.elf_shentsize);
        Elf32_Shdr *sec_hdr_32 = (Elf32_Shdr*)(m_data + m_info.elf_shoff + i * m_info.elf_shentsize);
        sec_name = (m_info.elf_is_x64 ? sec_hdr_64->sh_name : sec_hdr_32->sh_name) + m_info.elf_shstr;
        if (sec_name == ".dynamic")
        {
            m_info.elf_dynoff = m_info.elf_is_x64 ? sec_hdr_64->sh_offset : sec_hdr_32->sh_offset;
            m_info.elf_dynsize = m_info.elf_is_x64 ? sec_hdr_64->sh_size : sec_hdr_32->sh_size;
        }
        else if (sec_name == ".dynstr")
        {
            m_info.elf_dynstr = m_data + (m_info.elf_is_x64 ? sec_hdr_64->sh_offset: sec_hdr_32->sh_offset);
        }
        else if (sec_name == ".symtab")
        {
            m_info.elf_symtab_off = m_info.elf_is_x64 ? sec_hdr_64->sh_offset : sec_hdr_32->sh_offset;
            m_info.elf_symtab_link = m_info.elf_is_x64 ? sec_hdr_64->sh_link : sec_hdr_32->sh_link;
            m_info.elf_symtab_size = m_info.elf_is_x64 ? sec_hdr_64->sh_size : sec_hdr_32->sh_size;
        }
        else if (sec_name == ".dynsym")
        {
            m_info.elf_dynsym_off = m_info.elf_is_x64 ? sec_hdr_64->sh_offset : sec_hdr_32->sh_offset;
            m_info.elf_dynsym_size = m_info.elf_is_x64 ? sec_hdr_64->sh_size : sec_hdr_32->sh_size;
            m_info.elf_dynsym_link = m_info.elf_is_x64 ? sec_hdr_64->sh_link : sec_hdr_32->sh_link;;
        }
        tmp.sprintf("Section Header Table[%d]\t%s", i,sec_name.toLatin1().data());
        QTreeWidgetItem *item = new QTreeWidgetItem(root);
        item->setText(0, tmp);
        item->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_SECTION_ITEM);
        item->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_shoff + i * m_info.elf_shentsize);
        item->setData(0, ITEM_DATA_ITEM_SIZE, m_info.elf_shentsize);
    }
    return ELF_SUCCESS;
}

int ElfReader::init_dynamic_section()
{
    if (m_info.elf_dynoff == 0)
        return ELF_SUCCESS;
    QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
    QString tmp;
    root->setText(0, ".dynamic Detail");
    root->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_DYNAMIC_HEADER);
    root->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_dynoff);   
    root->setData(0, ITEM_DATA_ITEM_SIZE, m_info.elf_dynsize);
    return ELF_SUCCESS;
}


void *ElfReader::get_section_data(int index)
{
    Elf64_Shdr *sec_hdr_64 = (Elf64_Shdr*)(m_data + m_info.elf_shoff + index * m_info.elf_shentsize);
    Elf32_Shdr *sec_hdr_32 = (Elf32_Shdr*)(m_data + m_info.elf_shoff + index * m_info.elf_shentsize);
    int sec_offset = m_info.elf_is_x64 ? sec_hdr_64->sh_offset : sec_hdr_32->sh_offset;
    return (void *)(m_data + sec_offset);
}

int ElfReader::init_symbol_section()
{
    if (m_info.elf_symtab_off == 0)
        return ELF_SUCCESS;
    QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
    QString tmp;
    root->setText(0, ".symtab Section Detail");
    root->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_SYMTAB_HEADER);
    root->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_symtab_off);
    root->setData(0, ITEM_DATA_ITEM_SIZE, m_info.elf_symtab_size);
    QString sym_name;
    char *strtab = (char *)get_section_data(m_info.elf_symtab_link);
    int sym_entsize = m_info.elf_is_x64 ? sizeof(Elf64_Sym) : sizeof(Elf32_Sym);
    int sym_num = m_info.elf_is_x64 ? (m_info.elf_symtab_size / sizeof(Elf64_Sym)) : (m_info.elf_symtab_size / sizeof(Elf32_Sym));  
    for (int i = 0; i < sym_num; i++)
    {
        Elf64_Sym *sym_hdr_64 = (Elf64_Sym *)(m_data + m_info.elf_symtab_off + i * sym_entsize);
        Elf32_Sym *sym_hdr_32 = (Elf32_Sym *)(m_data + m_info.elf_symtab_off + i * sym_entsize);
        sym_name = (m_info.elf_is_x64 ? sym_hdr_64->st_name : sym_hdr_32->st_name) + strtab;
        if (sym_name.size() == 0)
            sym_name = "Undefined";
        tmp.sprintf(".symtab Table[%d]\t%s", i, sym_name.toLatin1().data());
        QTreeWidgetItem *item = new QTreeWidgetItem(root);
        item->setText(0, tmp);
        item->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_SYMTAB_ITEM);
        item->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_symtab_off + i * sym_entsize);
        item->setData(0, ITEM_DATA_ITEM_SIZE, sym_entsize);
    }
    return ELF_SUCCESS;
}

int ElfReader::init_dynsym_section()
{
    if (m_info.elf_dynsym_off == 0)
        return ELF_SUCCESS;
    QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
    QString tmp;
    root->setText(0, ".dynsym Section Detail");
    root->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_SYMTAB_HEADER);
    root->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_dynsym_off);
    root->setData(0, ITEM_DATA_ITEM_SIZE, m_info.elf_dynsym_size);
    QString sym_name;
    char *strtab = (char *)get_section_data(m_info.elf_dynsym_link);
    int sym_entsize = m_info.elf_is_x64 ? sizeof(Elf64_Sym) : sizeof(Elf32_Sym);
    int sym_num = m_info.elf_is_x64 ? (m_info.elf_dynsym_size / sizeof(Elf64_Sym)) : (m_info.elf_dynsym_size / sizeof(Elf32_Sym));
    for (int i = 0; i < sym_num; i++)
    {
        Elf64_Sym *sym_hdr_64 = (Elf64_Sym *)(m_data + m_info.elf_dynsym_off + i * sym_entsize);
        Elf32_Sym *sym_hdr_32 = (Elf32_Sym *)(m_data + m_info.elf_dynsym_off + i * sym_entsize);
        sym_name = (m_info.elf_is_x64 ? sym_hdr_64->st_name : sym_hdr_32->st_name) + strtab;
        if (sym_name.size() == 0)
            sym_name = "Undefined";
        tmp.sprintf(".dynsym Table[%d]\t%s", i, sym_name.toLatin1().data());
        QTreeWidgetItem *item = new QTreeWidgetItem(root);
        item->setText(0, tmp);
        item->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_SYMTAB_ITEM);
        item->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_dynsym_off + i * sym_entsize);
        item->setData(0, ITEM_DATA_ITEM_SIZE, sym_entsize);
    }
    return ELF_SUCCESS;
}