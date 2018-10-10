#ifndef ELF_READER_H
#define ELF_READER_H

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// ElfReaderApp: 
// 有关此类的实现，请参阅 reverse_tool.cpp
//

class ElfReaderApp : public CWinApp
{
public:
	ElfReaderApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern ElfReaderApp theApp;
#endif // ELF_READER_H

