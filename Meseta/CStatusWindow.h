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
	// 追加のメッセージハンドラ
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnNcDestroy();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();	

public:
	// コントロールの変数
	CBrush m_brDlg;
	COLORREF m_fontColor;

public:
	// ステータスウィンドウ更新関数

	// 背景色変更
	void SetAlpha(BYTE alpha);
	void SetColor(COLORREF c );
	void SetColor(BYTE r, BYTE g, BYTE b) { SetColor(RGB(r,g,b)); }

	// フォントカラー変更
	void SetFontColor(COLORREF c);
	void SetFontColor(BYTE r, BYTE g, BYTE b) { SetFontColor(RGB(r, g, b)); }

	// 色変更統合
	void SetColor(COLORREF dlgColor, COLORREF fontColor);

	// 経過時間の変更
	void SetTimeCount(CString time);
	void SetTimeCount(int h, int m, int s) {
		CString time;
		time.Format(L"%02d:%02d:%02d", h, m, s);
		SetTimeCount(time);
	}

	// 総メセタ更新
	void SetTotalMeseta(CString meseta);
	void SetTotalMeseta(long long meseta, long long mph);

	// ログの更新
	void clearLog();
	void SetLog(const std::vector <MesetaData>& mesetaData, bool change);

	// 情報テキスト更新
	void SetInfo(CString info);
	
	// 最前面設定
	void setTop(bool b, int top=-1, int left=-1);	

	// フォント設定
	void setCtrlFont(CFont* font);
};
