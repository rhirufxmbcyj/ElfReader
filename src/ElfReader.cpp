#include "ElfReader.h"

ElfReader::ElfReader(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//设置splitter比例 左边占2份 右边占5份
	ui.splitter_left->setStretchFactor(0, 2);
	ui.splitter_left->setStretchFactor(1, 5);
	//连接analyze信号槽
	QObject::connect(this, SIGNAL(start_analyze(QString)), this, SLOT(start_analyze_slot(QString)));
}

void ElfReader::clean_up_data()
{
	if (file)
	{
		delete file;
		file = NULL;
	}
	//清空list tree控件
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
	clean_up_data();
	QFile f(file_name);
	if (!f.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(NULL, MESSAGE_OPEN_FILE_ERROR, MESSAGE_CAPTION);
	}
	//读文件 最好读到char*里边
}
