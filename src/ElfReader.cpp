#include "ElfReader.h"

ElfReader::ElfReader(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//设置splitter比例 左边占2份 右边占5份
	ui.splitter_left->setStretchFactor(0, 2);
	ui.splitter_left->setStretchFactor(1, 5);
}

void ElfReader::dropEvent(QDropEvent *event)
{
	qDebug() << "drop";
}

void ElfReader::pushButton_open_clicked()
{
	qDebug() << "open";
}

void ElfReader::pushButton_save_clicked()
{
	qDebug() << "save";
}
