#include "ElfReader.h"
#include <assert.h>
ElfReader::ElfReader(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//设置splitter比例 左边占2份 右边占5份
	ui.splitter_left->setStretchFactor(0, 2);
	ui.splitter_left->setStretchFactor(1, 5);
	//连接analyze信号槽
	QObject::connect(this, SIGNAL(start_analyze(QString)), this, SLOT(start_analyze_slot(QString)));
	QObject::connect(ui.header_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(header_item_changed_slot(QTreeWidgetItem*, QTreeWidgetItem*)));
	m_data = NULL;
	QStringList info_header;
	info_header << "Name" << "Value" << "Start" << "Size";
	ui.info_tree->setHeaderLabels(info_header);
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

void ElfReader::pushButton_open_clicked()
{
	QString file_name = QFileDialog::getOpenFileName(this,u8"请选择一个ELF文件","","",0);
	//qDebug() << file_name;
	emit start_analyze(file_name);
}

void ElfReader::pushButton_save_clicked()
{
	qDebug() << transform_string(abcdk);
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
		QMessageBox::warning(ui.centralWidget, MESSAGE_CAPTION, MESSAGE_OPEN_FILE_ERROR);
		goto End;
	}
	file_data = file.readAll();
	m_data = new char[file_data.size()];
	memcpy(m_data, file_data.data(), file_data.size());
	//检查文件合法性
	if (check_file_format() != ELF_SUCCESS)
	{
		QMessageBox::warning(ui.centralWidget, MESSAGE_CAPTION, MESSAGE_FILE_FORMAT_ERROR);
		goto End;
	}
	parse_elf();
	init_header_tree();
End:
	return;
}

void ElfReader::header_item_changed_slot(QTreeWidgetItem *current_item, QTreeWidgetItem *pre_item)
{
	int header_type = current_item->data(0, ITEM_DATA_ITEM_TYPE).toInt();
	int offset = current_item->data(0, ITEM_DATA_ITEM_OFFSET).toInt();
	switch (header_type)
	{
	case ITEM_DATA_ELF_HEADER:
		init_elf_header_info(offset);
		break;
	case ITEM_DATA_ELF_PROGRAM_HEADER:
		//不处理
		break;
	case ITEM_DATA_ELF_PROGRAM_ITEM:
		break;
	case ITEM_DATA_ELF_SECTION_HEADER:
		//不处理
		break;
	case ITEM_DATA_ELF_SECTION_ITEM:
		break;
	default:assert(0); break;
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
		Elf64_Ehdr *elf_header = (Elf64_Ehdr*)m_data;
		m_info.elf_type = elf_header->e_type;
		m_info.elf_phoff = elf_header->e_phoff;
		m_info.elf_phentsize = elf_header->e_phentsize;
		m_info.elf_phnum = elf_header->e_phnum;
		m_info.elf_shoff = elf_header->e_shoff;
		m_info.elf_shentsize = elf_header->e_shentsize;
		m_info.elf_shnum = elf_header->e_shnum;
		m_info.elf_shstrndx = elf_header->e_shstrndx;
	}
	else
	{
		Elf32_Ehdr *elf_header = (Elf32_Ehdr*)m_data;
		m_info.elf_type = elf_header->e_type;
		m_info.elf_phoff = elf_header->e_phoff;
		m_info.elf_phentsize = elf_header->e_phentsize;
		m_info.elf_phnum = elf_header->e_phnum;
		m_info.elf_shoff = elf_header->e_shoff;
		m_info.elf_shentsize = elf_header->e_shentsize;
		m_info.elf_shnum = elf_header->e_shnum;
		m_info.elf_shstrndx = elf_header->e_shstrndx;
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
	for (int i = 0; i < m_info.elf_phnum; i++)
	{
		tmp.sprintf("Program Header Table[%d]", i);
		QTreeWidgetItem *item = new QTreeWidgetItem(root);
		item->setText(0,tmp);
		item->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_PROGRAM_ITEM);
		item->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_phoff + i * m_info.elf_phentsize);
	}
	return ELF_SUCCESS;
}

int ElfReader::init_section_header()
{
	if (m_info.elf_shnum == 0)
		return ELF_SUCCESS;
	QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
	QString tmp;
	root->setText(0, "Section Header Table");
	root->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_SECTION_HEADER);
	root->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_shoff);
	for (int i = 0; i < m_info.elf_shnum; i++)
	{
		//要加上section名
		tmp.sprintf("Section Header Table[%d]", i);
		QTreeWidgetItem *item = new QTreeWidgetItem(root);
		item->setText(0, tmp);
		item->setData(0, ITEM_DATA_ITEM_TYPE, ITEM_DATA_ELF_SECTION_ITEM);
		item->setData(0, ITEM_DATA_ITEM_OFFSET, m_info.elf_shoff + i * m_info.elf_shentsize);
	}
	return ELF_SUCCESS;
}

int ElfReader::init_elf_header_info(int offset)
{
	Elf64_Ehdr *elf64_header = (Elf64_Ehdr *)m_data;
	Elf32_Ehdr *elf32_header = (Elf32_Ehdr *)m_data;
	unsigned char *p = (unsigned char *)m_data;
	QString tmp;

	QTreeWidgetItem *root = new QTreeWidgetItem(ui.info_tree);
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
	item->setText(2, QString::number(offset+ EI_CLASS));
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
	return 0;
}
