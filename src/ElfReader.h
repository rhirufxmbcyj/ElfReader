#ifndef ELF_READER_H
#define ELF_READER_H

#include <QtWidgets/QMainWindow>
#include "ui_ElfReader.h"
#include "define.h"
//拖拽取文件路径
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
//文件选择对话框
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>
//hexedit
#include "qhexedit2/qhexedit.h"
typedef struct _elf_info_st
{
    _elf_info_st()
    {
        memset(this, 0, sizeof(_elf_info_st));
    }
    int elf_type;
    int elf_is_x64;
    int elf_ehsize;

    int elf_phoff;
    int elf_phentsize;
    int elf_phnum;

    int elf_shoff;
    int elf_shentsize;
    int elf_shnum;

    int elf_dynoff;
    int elf_dynsize;

    char *elf_shstr;
    char *elf_dynstr;
    char *elf_strtab;

    int elf_symtab_off;
    int elf_symtab_link;
    int elf_symtab_size;

    int elf_dynsym_off;
    int elf_dynsym_link;
    int elf_dynsym_size;
}elf_info_st;


class ElfReader : public QMainWindow
{
    Q_OBJECT

public:
    ElfReader(QWidget *parent = Q_NULLPTR);
    char *m_data;
    elf_info_st m_info;
    Ui::ElfReader ui;
    QHexEdit *hexEdit;
protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void paintEvent(QPaintEvent *event);
public slots:
    void pushButton_open_clicked();
    void pushButton_save_clicked();
    void start_analyze_slot(QString file_name);
    void header_item_changed_slot(QTreeWidgetItem *current_item, QTreeWidgetItem *pre_item);
    void info_item_changed_slot(QTreeWidgetItem *current_item, QTreeWidgetItem *pre_item);
signals:
    void start_analyze(QString file_name);
public:
    void init_info_header(int type);
    void clean_up_data();
    int check_file_format();
    void parse_elf();

    int init_header_tree();
    int init_elf_header();
    int init_program_header();
    int init_section_header();
    int init_dynamic_section();
    void *get_section_data(int index);
    int init_symbol_section();
    int init_dynsym_section();
};

#endif
