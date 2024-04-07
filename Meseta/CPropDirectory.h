// CPropDirectory ダイアログ
// 
// 設定・動作ディレクトリ
//
#pragma once

#include "afxdialogex.h"
#include "MesetaDlg.h"


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
	// 追加のメッセージハンドラ	
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEnChangeEditLogNgs();
	afx_msg void OnBnClickedButtonLogNgs();
	afx_msg void OnBnClickedButtonSaveDir();
	afx_msg void OnEnChangeEditSaveDir();

public:
	// コントロール変数
	CEdit m_edit_log_ngs;
	CEdit m_edit_directory;
	CEdit m_edit_save_dir;	
};
