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
	m_data = NULL;
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
	qDebug() << "save";
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
	root->setData(0, ITEM_DATA_HEADER_TYPE, ITEM_DATA_ELF_HEADER);
	return ELF_SUCCESS;
}

int ElfReader::init_program_header()
{
	QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
	QString tmp;
	root->setText(0, "Program Header Table");
	root->setData(0, ITEM_DATA_HEADER_TYPE, ITEM_DATA_ELF_PROGRAM_HEADER);
	for (int i = 0; i < m_info.elf_phnum; i++)
	{
		tmp.sprintf("Program Header Table[%d]", i);
		QTreeWidgetItem *item = new QTreeWidgetItem(root);
		item->setText(0,tmp);
		item->setData(0, ITEM_DATA_HEADER_TYPE, ITEM_DATA_ELF_PROGRAM_ITEM);
	}
	return ELF_SUCCESS;
}

int ElfReader::init_section_header()
{
	QTreeWidgetItem *root = new QTreeWidgetItem(ui.header_tree);
	QString tmp;
	root->setText(0, "Section Header Table");
	root->setData(0, ITEM_DATA_HEADER_TYPE, ITEM_DATA_ELF_SECTION_HEADER);
	for (int i = 0; i < m_info.elf_shnum; i++)
	{
		//要加上section名
		tmp.sprintf("Section Header Table[%d]", i);
		QTreeWidgetItem *item = new QTreeWidgetItem(root);
		item->setText(0, tmp);
		item->setData(0, ITEM_DATA_HEADER_TYPE, ITEM_DATA_ELF_SECTION_ITEM);
	}
	return ELF_SUCCESS;
}
