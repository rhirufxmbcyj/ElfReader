#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ElfReader.h"

class ElfReader : public QMainWindow
{
	Q_OBJECT

public:
	ElfReader(QWidget *parent = Q_NULLPTR);

private:
	Ui::ElfReaderClass ui;
};
