
// MesetaDlg.h : ヘッダー ファイル
//
#pragma once

#include "CStatusWindow.h"
#include "MesetaData.h"
#include "INI_INFO.h"
#include "PAD_INFO.h"


// CMesetaDlg ダイアログ
class CMesetaDlg : public CDialogEx
{
// コンストラクション
public:
	CMesetaDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESETA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 追加のメッセージハンドラ
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnDestroy();
	afx_msg void OnClose();

	afx_msg void OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedCheckPreSt();
	afx_msg void OnBnClickedCheckPreLog();

	afx_msg void OnBnClickedCheckTraLog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnBnClickedCheckAuto();
		
	afx_msg void OnBnClickedButtonEnd();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonConfig();

public:
	// コントローラの変数
	CListCtrl m_listCtrl;
	CButton m_check_pre_status;
	CButton m_check_pre_log;
	CButton m_check_trans_log;
	CSliderCtrl m_window_alpha;
	CButton m_check_use_auto_finish;
	CComboBox m_auto_finish_count;
	CButton m_buttonStart;
	CButton m_buttonEnd;
	CButton m_buttonConfig;

public:
	// 独自の追加変数
	MesetaDataCtrl mesetaCtrl;
	CStatusWindow* m_statusWindow;
	UINT m_timerID;
	UINT m_autoFinishTimerID;
	UINT m_padTimerID;
	PAD_INFO padInfo;
	INI_FILE iniData;
	
public:
	// 独自追加関数
	bool SetHotkey();
	bool DeleteHotkey();
	void clearStatusWindow();
	void openStatusWindow();
	bool checkIniFile();
	BOOL readINI();
	BOOL writeINI();
	void Init(bool run);
	void finishRecData(long long currentMeseta);	
	
public:
	// ユーティリティ
	static COLORREF Str2Col(CString col);
	static CString Col2Str(COLORREF col);
	static bool Str2VK(CString key, UINT& mod, UINT& vk);
};
