// ElfReaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ElfReader.h"
#include "ElfReaderDlg.h"
#include "afxdialogex.h"


//tree的item可以存放信息 SetItemData()函数   可以用一个结构体保存多个信息 参数存入结构体的地址
//在tree中显示section名
// ElfReaderDlg 对话框

IMPLEMENT_DYNAMIC(ElfReaderDlg, CDialogEx)

ElfReaderDlg::ElfReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

ElfReaderDlg::~ElfReaderDlg()
{

}


void ElfReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}

void ElfReaderDlg::PaserElf()
{
	if (file == NULL)
		return;
	elf_info.elf_is_x64 = file[EI_CLASS] == ELFCLASS64 ? TRUE : FALSE;
	if (elf_info.elf_is_x64)
	{
		Elf64_Ehdr *elf_header = (Elf64_Ehdr*)file;
		elf_info.elf_type = elf_header->e_type;
		elf_info.elf_phoff = elf_header->e_phoff;
		elf_info.elf_phentsize = elf_header->e_phentsize;
		elf_info.elf_phnum = elf_header->e_phnum;
		elf_info.elf_shoff = elf_header->e_shoff;
		elf_info.elf_shentsize = elf_header->e_shentsize;
		elf_info.elf_shnum = elf_header->e_shnum;
		elf_info.elf_shstrndx = elf_header->e_shstrndx;
	}
	else
	{
		Elf32_Ehdr *elf_header = (Elf32_Ehdr*)file;
		elf_info.elf_type = elf_header->e_type;
		elf_info.elf_phoff = elf_header->e_phoff;
		elf_info.elf_phentsize = elf_header->e_phentsize;
		elf_info.elf_phnum = elf_header->e_phnum;
		elf_info.elf_shoff = elf_header->e_shoff;
		elf_info.elf_shentsize = elf_header->e_shentsize;
		elf_info.elf_shnum = elf_header->e_shnum;
		elf_info.elf_shstrndx = elf_header->e_shstrndx;
	}
}

void ElfReaderDlg::InitTreeCtrl()
{
	if (InitElfHeader() != ELF_OK)
		goto End;
	if (InitElfProgramHeader() != ELF_OK)
		goto End;
	if (InitElfSectionHeader() != ELF_OK)
		goto End;

End:
	return;
}

int ElfReaderDlg::InitElfHeader()
{
	HTREEITEM hRoot;
	hRoot = m_tree.InsertItem(L"Elf Header", TVI_ROOT, TVI_LAST);
	item_data_t *data = new item_data_t();
	data->type = ITEM_DATA_ELF_HEADER;
	m_tree.SetItemData(hRoot, (DWORD_PTR)data);
	return ELF_OK;
}

int ElfReaderDlg::InitElfHeaderWindow()
{
	return ELF_OK;
	
}

int ElfReaderDlg::InitElfProgramHeader()
{
	HTREEITEM hRoot = m_tree.InsertItem(L"Program Header Table", TVI_ROOT, TVI_LAST);
	item_data_t *data = new item_data_t();
	data->type = ITEM_DATA_NOTHING;
	m_tree.SetItemData(hRoot, (DWORD_PTR)data);

	for (int i = 0; i < elf_info.elf_phnum; i++)
	{
		WCHAR tmp[64] = { 0 };
		wsprintf(tmp, L"Program Header Table[%d]", i);
		HTREEITEM item = m_tree.InsertItem(tmp, hRoot, TVI_LAST);
		item_data_t *data = new item_data_t();
		data->type = ITEM_DATA_ELF_PROGRAM_HEADER;
		m_tree.SetItemData(item, (DWORD_PTR)data);
	}

	return ELF_OK;
}

int ElfReaderDlg::InitElfProgramHeaderWindow()
{
	return ELF_OK;
}

int ElfReaderDlg::InitElfSectionHeader()
{
	HTREEITEM hRoot = m_tree.InsertItem(L"Section Header Table", TVI_ROOT, TVI_LAST);
	item_data_t *data = new item_data_t();
	data->type = ITEM_DATA_NOTHING;
	m_tree.SetItemData(hRoot, (DWORD_PTR)data);

	for (int i = 0; i < elf_info.elf_shnum; i++)
	{
		WCHAR tmp[64] = { 0 };
		wsprintf(tmp, L"Section Header Table[%d]", i);
		HTREEITEM item = m_tree.InsertItem(tmp, hRoot, TVI_LAST);
		item_data_t *data = new item_data_t();
		data->type = ITEM_DATA_ELF_SECTION_HEADER;
		m_tree.SetItemData(item, (DWORD_PTR)data);
	}

	return ELF_OK;
}

int ElfReaderDlg::InitElfSectionHeaderWindow()
{

	return ELF_OK;
}

BOOL ElfReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	file = NULL;
	memset(&elf_info, 0, sizeof(elf_info_t));
	return TRUE;  
}


int ElfReaderDlg::CheckFileFormat()
{
	assert(file);
	if (*((DWORD*)file) != *((DWORD*)ELFMAG))
		return ELF_INVALID_FORMAT;
	return ELF_OK;
}

void ElfReaderDlg::DeleteItemData(HTREEITEM hRootitem)
{
	HTREEITEM hitem = m_tree.GetNextItem(hRootitem, TVGN_CHILD);
	while (hitem != NULL)
	{
		DeleteItemData(hitem);
		item_data_t *data = (item_data_t *)m_tree.GetItemData(hitem);
		assert(data);
		delete data;
		hitem = m_tree.GetNextItem(hitem, TVGN_NEXT);
	}
}

void ElfReaderDlg::CleanUpData()
{
	if (file)
	{
		delete file;
		file = NULL;
	}
	DeleteItemData(NULL);
	m_tree.DeleteAllItems();
}

BOOL ElfReaderDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(ElfReaderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ElfReaderDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_START_ANALYZE,OnStartAnalyze)
	ON_WM_DROPFILES()
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, &ElfReaderDlg::OnTvnSelchangingTree1)
END_MESSAGE_MAP()


// ElfReaderDlg 消息处理程序




void ElfReaderDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_FileName = dlg.GetPathName(); //文件名保存在了FilePathName里
		PostMessage(WM_START_ANALYZE, NULL, NULL);
	}
}

LRESULT ElfReaderDlg::OnStartAnalyze(WPARAM wParam, LPARAM lParam)
{
	DWORD dwReadSize = 0;
	//清除上次数据
	CleanUpData();
	HANDLE hFile = CreateFile(m_FileName.GetBuffer(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(MESSAGE_OPEN_FILE_ERROR, MESSAGE_CAPTION);
		goto End;
	}
	dwFileSize = GetFileSize(hFile, NULL);
	file = new char[dwFileSize];
	ReadFile(hFile, file, dwFileSize, &dwReadSize, NULL);
	if (dwReadSize != dwFileSize)
	{
		MessageBox(MESSAGE_READ_FILE_ERROR, MESSAGE_CAPTION);
		goto End;
	}
	//检查文件合法性
	if (CheckFileFormat() != ELF_OK)
	{
		MessageBox(MESSAGE_FILE_FORMAT_ERROR, MESSAGE_CAPTION);
		goto End;
	}
	PaserElf();
	//初始化控件
	InitTreeCtrl();
End:
	CloseHandle(hFile);
	return 0;
}


void ElfReaderDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int DropCount=DragQueryFile(hDropInfo,-1,NULL,0);
	//取得被拖动文件的数目	
	WCHAR wcStr[MAX_PATH];		
	DragQueryFile(hDropInfo,0,wcStr,MAX_PATH);
	m_FileName = wcStr;
	DragFinish(hDropInfo);
	PostMessage(WM_START_ANALYZE, NULL, NULL);
	CDialogEx::OnDropFiles(hDropInfo);
}


void ElfReaderDlg::OnTvnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM item = pNMTreeView->itemNew.hItem;
	item_data_t *item_data = (item_data_t *)m_tree.GetItemData(item);
	switch (item_data->type)
	{
	case ITEM_DATA_NOTHING:
		break;
	case ITEM_DATA_ELF_HEADER:
		InitElfHeaderWindow();
		break;
	case ITEM_DATA_ELF_PROGRAM_HEADER:
		InitElfProgramHeaderWindow();
		break;
	case ITEM_DATA_ELF_SECTION_HEADER:
		InitElfSectionHeaderWindow();
		break;
	default:
		MessageBox(MESSAGE_NO_TYPE, MESSAGE_CAPTION);
		break;
	}
	*pResult = 0;
}
