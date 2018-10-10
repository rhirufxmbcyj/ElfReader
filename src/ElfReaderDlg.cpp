// ElfReaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ElfReader.h"
#include "ElfReaderDlg.h"
#include "afxdialogex.h"


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

void ElfReaderDlg::InitTreeCtrl()
{

}

BOOL ElfReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitTreeCtrl();

	return TRUE;  
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
	//检查文件合法 清空各个数据 然后初始化控件
	MessageBoxW(m_FileName.GetBuffer());
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
	//拖放结束后,释放内存
	PostMessage(WM_START_ANALYZE, NULL, NULL);
	CDialogEx::OnDropFiles(hDropInfo);
}
