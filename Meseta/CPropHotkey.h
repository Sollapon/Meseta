// CPropHotkey ダイアログ
// 
// 設定・ホットキータブ
//
#pragma once

#include "afxdialogex.h"
#include "MesetaDlg.h"


class CPropHotkey : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropHotkey)

public:
	CPropHotkey(CMesetaDlg* pParent);   // 標準コンストラクター
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
	// 追加のメッセージハンドラ
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnBnClickedHkShiht();
	afx_msg void OnBnClickedHkCtrl();
	afx_msg void OnBnClickedHkAlt();
	afx_msg void OnBnClickedHkWin();
	afx_msg void OnCbnSelchangeComboHotkey();

	afx_msg void OnBnClickedPropCheckDelUse();
	afx_msg void OnBnClickedHkShift2();
	afx_msg void OnBnClickedHkCtrl2();
	afx_msg void OnBnClickedHkAlt2();
	afx_msg void OnBnClickedHkWin2();
	afx_msg void OnCbnSelchangeComboHotkey2();
	virtual BOOL OnSetActive();

public:
	// コントロールの変数
	CButton m_rec_shift;
	CButton m_rec_ctrl;
	CButton m_rec_alt;
	CButton m_rec_win;
	CComboBox m_comb_vk;

	CButton m_check_use_del;
	CButton m_del_shift;
	CButton m_del_ctrl;
	CButton m_del_alt;
	CButton m_del_win;
	CComboBox m_comb_vk_del;

	std::vector< CButton*> check_button[2];
	std::vector< CComboBox*> combo_ctrl;

public:
	bool checkInvalid(int idx);
	void useHotkeyDel(BOOL use);
	CString makeHotkeyStr(int idx);	
};
