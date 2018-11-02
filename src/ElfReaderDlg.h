#ifndef ELF_READER_DLG_H
#define ELF_READER_DLG_H

#include <define.h>
#include "elf.h"
#include "afxcmn.h"


// ElfReaderDlg 对话框

class ElfReaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ElfReaderDlg)

public:
	ElfReaderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ElfReaderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	void InitTreeCtrl();
	int InitElfHeader();
	CTreeCtrl m_tree;
	CString m_FileName;
	virtual BOOL OnInitDialog();
	int CheckFileFormat();
	void CleanUpData();
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnStartAnalyze(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDropFiles(HDROP hDropInfo);
private:
	char *file;
	DWORD dwFileSize;
public:
	afx_msg void OnTvnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif
