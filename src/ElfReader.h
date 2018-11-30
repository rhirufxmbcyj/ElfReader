#ifndef ELF_READER_H
#define ELF_READER_H

#include <QtWidgets/QMainWindow>
#include "ui_ElfReader.h"
#include "define.h"

class ElfReader : public QMainWindow
{
	Q_OBJECT

public:
	ElfReader(QWidget *parent = Q_NULLPTR);

private:
	Ui::ElfReader ui;
protected:
	void dropEvent(QDropEvent *event);
public slots:
	void pushButton_open_clicked();
	void pushButton_save_clicked();
};

#endif
