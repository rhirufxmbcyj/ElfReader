#ifndef DEFINE_H
#define DEFINE_H

#define		WM_START_ANALYZE	WM_USER + 100


#define		MESSAGE_CAPTION				L"提示"
#define		MESSAGE_OPEN_FILE_ERROR		L"文件打开失败"
#define		MESSAGE_READ_FILE_ERROR		L"文件读取失败"
#define		MESSAGE_FILE_FORMAT_ERROR   L"文件读取失败"
#define		MESSAGE_NO_TYPE				L"没有这种类型，请将文件及错误提交issue"


#define		ELF_OK						0
#define		ELF_INVALID_FORMAT			-1


typedef struct _ITEM_DATA
{
	int type;

}ITEM_DATA;

#define		ITEM_DATA_ELF_HEADER	1
#endif