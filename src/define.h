#ifndef DEFINE_H
#define DEFINE_H

#ifdef _DEBUG
#include <qDebug>
#endif

#include "elf.h"

#define		WM_START_ANALYZE	WM_USER + 100


#define		MESSAGE_CAPTION				u8"提示"
#define		MESSAGE_OPEN_FILE_ERROR		u8"文件打开失败"
#define		MESSAGE_READ_FILE_ERROR		u8"文件读取失败"
#define		MESSAGE_FILE_FORMAT_ERROR   u8"文件读取失败"
#define		MESSAGE_NO_TYPE				u8"没有这种类型，请将文件及错误提交issue"


#define		ELF_OK						 0
#define		ELF_INVALID_FORMAT			-1


typedef struct _item_data_t
{
	int type;

}item_data_t;

#define		ITEM_DATA_NOTHING				0	
#define		ITEM_DATA_ELF_HEADER			1
#define		ITEM_DATA_ELF_PROGRAM_HEADER	2
#define		ITEM_DATA_ELF_SECTION_HEADER	3

#endif