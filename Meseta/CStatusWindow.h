#pragma once
#include "afxdialogex.h"
#include "MesetaData.h"

// CStatusWindow ダイアログ

class CStatusWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusWindow)

public:
	CStatusWindow(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CStatusWindow();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcDestroy();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CBrush m_brDlg;
	COLORREF m_fontColor;

	void SetAlpha(BYTE alpha);
	void SetColor(COLORREF c );
	void SetColor(COLORREF dlgColor, COLORREF fontColor);
	void SetColor(BYTE r, BYTE g, BYTE b) { SetColor(RGB(r,g,b)); }

	void SetFontColor(COLORREF c);
	void SetFontColor(BYTE r, BYTE g, BYTE b) { SetFontColor(RGB(r, g, b)); }

	void SetTimeCount(CString time);
	void SetTimeCount(int h, int m, int s) {
		CString time;
		time.Format(L"%02d:%02d:%02d", h, m, s);
		SetTimeCount(time);
	}

	void SetTotalMeseta(CString meseta);
	void SetTotalMeseta(long long meseta, long long mph);

	void clearLog();
	void SetLog(const std::vector <MesetaData>& mesetaData, bool change);
	
	void setTop(bool b, int top=-1, int left=-1);

	void SetInfo(CString info);
};
