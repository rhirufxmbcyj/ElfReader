#include "ElfReader.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ElfReader w;
	w.show();
	return a.exec();
}
