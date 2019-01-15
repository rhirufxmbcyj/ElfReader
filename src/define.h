#ifndef DEFINE_H
#define DEFINE_H

#ifdef _DEBUG
#include <qDebug>
#endif

#include "elf.h"

#define        transform_string(a)            #a

#define        MESSAGE_CAPTION                u8"提示"
#define        MESSAGE_OPEN_FILE_ERROR        u8"文件打开失败"
#define        MESSAGE_READ_FILE_ERROR        u8"文件读取失败"
#define        MESSAGE_FILE_FORMAT_ERROR      u8"该文件不是合法的ELF文件"
#define        MESSAGE_NO_TYPE                u8"没有这种类型，请将文件及错误提交issue"
#define        MESSAGE_TYPE_ERROR             u8"类型解析错误，请将文件及错误提交issue"
#define        MESSAGE_HEX_ERROR              u8"十六进制编辑器无法读取文件内容，请将文件及错误提交issue"

#define        TRUE                            1
#define        FALSE                           0

#define        ELF_SUCCESS                     0
#define        ELF_INVALID_FORMAT             -1

//TreeWidget setData用到的宏
#define        ITEM_DATA_ITEM_TYPE                Qt::UserRole + 1
#define        ITEM_DATA_ELF_HEADER               1
#define        ITEM_DATA_ELF_PROGRAM_HEADER       2
#define        ITEM_DATA_ELF_PROGRAM_ITEM         3
#define        ITEM_DATA_ELF_SECTION_HEADER       4
#define        ITEM_DATA_ELF_SECTION_ITEM         5
#define        ITEM_DATA_ELF_DYNAMIC_HEADER       6
#define        ITEM_DATA_ELF_DYNAMIC_INFO         7

#define        ITEM_DATA_ITEM_OFFSET              Qt::UserRole + 2
#define        ITEM_DATA_ITEM_SIZE                Qt::UserRole + 3








// typedef struct _item_data_st
// {
//     _item_data_st()
//     {
//         memset(this, 0, sizeof(_item_data_st));
//     }
//     int type;
// }item_data_st;

#endif