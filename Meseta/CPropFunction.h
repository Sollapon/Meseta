#pragma once
#include "afxdialogex.h"

#include "MesetaDlg.h"


// CPropFunction ダイアログ

class CPropFunction : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropFunction)

public:
	CPropFunction(CMesetaDlg* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropFunction();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_FUNCTION };
#endif

private:
	CMesetaDlg* parentDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckAutoRefresh();
	afx_msg void OnCbnSelchangeComboAutoRefresh();
	CButton m_check_auto_refresh;
	CComboBox m_combo_auto_refresh;
	CButton m_check_save_pos;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedCheckSavePos();
	afx_msg void OnEditchangeComboAutoRefresh();
};
