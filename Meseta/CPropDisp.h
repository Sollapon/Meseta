// CPropDisp ダイアログ
// 
// 設定：表示タブ
//
#pragma once

#include "afxdialogex.h"
#include "MesetaDlg.h"


class CPropDisp : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropDisp)

public:
	CPropDisp(CMesetaDlg* CMesetaDlg);   // 標準コンストラクター
	virtual ~CPropDisp();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DISP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	CMesetaDlg* parentDlg;

public:
	// コントロール変数
	CButton m_check_save_pos;
	CComboBox m_combo_log_num;
	CComboBox m_combo_sub_data;

public:
	// 追加のメッセージハンドラ
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	afx_msg void OnCbnSelchangeComboLogNum();
	afx_msg void OnCbnSelchangeComboSubData();
	afx_msg void OnBnClickedCheckSavePos();
};
