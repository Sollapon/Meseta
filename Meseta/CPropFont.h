// CPropFont ダイアログ
// 
// 設定：表示
//
#pragma once

#include "afxdialogex.h"
#include "MesetaDlg.h"


class CPropFont : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropFont)

public:
	CPropFont(CMesetaDlg* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropFont();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_FONT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	CMesetaDlg* parentDlg;
	LOGFONT logFont;
	FONT_INFO fontInfo;
	CFont sampleFont;
	CBrush m_brDlg;
	COLORREF m_fontColorN;
	COLORREF m_fontColorR;

public:
	// 追加のメッセージハンドラ
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonChangeFont();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
