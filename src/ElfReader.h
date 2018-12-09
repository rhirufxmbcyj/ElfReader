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

typedef struct _elf_info_st
{
    _elf_info_st()
    {
        memset(this, 0, sizeof(_elf_info_st));
    }
    int elf_type;
    int elf_is_x64;
    int elf_phoff;
    int elf_phentsize;
    int elf_phnum;
    int elf_shoff;
    int elf_shentsize;
    int elf_shnum;
    int elf_shstrndx;
}elf_info_st;


class ElfReader : public QMainWindow
{
    Q_OBJECT

public:
    ElfReader(QWidget *parent = Q_NULLPTR);
    char *m_data;
    elf_info_st m_info;
private:
    Ui::ElfReader ui;
protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
public slots:
    void pushButton_open_clicked();
    void pushButton_save_clicked();
    void start_analyze_slot(QString file_name);
    void header_item_changed_slot(QTreeWidgetItem *current_item, QTreeWidgetItem *pre_item);
signals:
    void start_analyze(QString file_name);
public:
    void clean_up_data();
    int check_file_format();
    void parse_elf();

    int init_header_tree();
    int init_elf_header();
    int init_program_header();
    int init_section_header();

    int init_elf_header_info(int offset);

};

#endif
