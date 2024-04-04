#pragma once
#include "afxdialogex.h"

#include "MesetaDlg.h"


// CPropHotkey ダイアログ

class CPropHotkey : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropHotkey)

public:
	CPropHotkey(CMesetaDlg* parent);   // 標準コンストラクター
	virtual ~CPropHotkey();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_HOTKEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	CMesetaDlg* parentDlg;
public:
	CButton m_rec_shift;
	CButton m_rec_ctrl;
	CButton m_rec_alt;
	CButton m_rec_win;
	bool checkInvalid();
	afx_msg void OnBnClickedHkShiht();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedHkCtrl();
	afx_msg void OnBnClickedHkAlt();
	afx_msg void OnBnClickedHkWin();
	afx_msg void OnCbnSelchangeComboHotkey();
	CComboBox m_comb_vk;
	virtual void OnOK();
};
