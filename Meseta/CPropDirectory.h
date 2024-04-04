#pragma once
#include "afxdialogex.h"

#include "MesetaDlg.h"


// CPropDirectory ダイアログ

class CPropDirectory : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropDirectory)

public:
	CPropDirectory(CMesetaDlg* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropDirectory();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	CMesetaDlg* parentDlg;
public:
	CEdit m_edit_log_ngs;
	afx_msg void OnEnChangeEditLogNgs();
	afx_msg void OnBnClickedButtonLogNgs();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CEdit m_edit_directory;
};
