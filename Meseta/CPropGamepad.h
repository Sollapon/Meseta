// CPropGamepad ダイアログ
// 
// 設定：ゲームパッド
//
#pragma once

#include "afxdialogex.h"
#include "MesetaDlg.h"


class CPropGamepad : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPropGamepad)

public:
	CPropGamepad(CMesetaDlg* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropGamepad();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_GAMEPAD };
#endif

private:
	CMesetaDlg* parentDlg;

	const WORD PAD_IDC[2][16] =
	{
		{
		0,
		IDC_CHECK_PAD1,
		IDC_CHECK_PAD2,
		IDC_CHECK_PAD3,
		IDC_CHECK_PAD4,
		IDC_CHECK_PAD5,
		IDC_CHECK_PAD6,
		IDC_CHECK_PAD7,
		IDC_CHECK_PAD8,
		IDC_CHECK_PAD9,
		IDC_CHECK_PAD10,
		IDC_CHECK_PAD11,
		IDC_CHECK_PAD12,
		IDC_CHECK_PAD13,
		IDC_CHECK_PAD14,
		0,
		},

		{
		0,
		IDC_CHECK_PAD15,
		IDC_CHECK_PAD16,
		IDC_CHECK_PAD17,
		IDC_CHECK_PAD18,
		IDC_CHECK_PAD19,
		IDC_CHECK_PAD20,
		IDC_CHECK_PAD21,
		IDC_CHECK_PAD22,
		IDC_CHECK_PAD23,
		IDC_CHECK_PAD24,
		IDC_CHECK_PAD25,
		IDC_CHECK_PAD26,
		IDC_CHECK_PAD27,
		IDC_CHECK_PAD28,
		0,
		}
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 追加のメッセージハンドラ
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckgamepadUse();
	afx_msg void OnCbnSelchangeComboGamepadId();

	afx_msg void OnBnClickedCheckPad1();
	afx_msg void OnBnClickedCheckPad2();
	afx_msg void OnBnClickedCheckPad3();
	afx_msg void OnBnClickedCheckPad4();
	afx_msg void OnBnClickedCheckPad5();
	afx_msg void OnBnClickedCheckPad6();
	afx_msg void OnBnClickedCheckPad7();
	afx_msg void OnBnClickedCheckPad8();
	afx_msg void OnBnClickedCheckPad9();
	afx_msg void OnBnClickedCheckPad10();
	afx_msg void OnBnClickedCheckPad11();
	afx_msg void OnBnClickedCheckPad12();
	afx_msg void OnBnClickedCheckPad13();
	afx_msg void OnBnClickedCheckPad14();
	afx_msg void OnBnClickedCheckPad15();
	afx_msg void OnBnClickedCheckPad16();
	afx_msg void OnBnClickedCheckPad17();
	afx_msg void OnBnClickedCheckPad18();
	afx_msg void OnBnClickedCheckPad19();
	afx_msg void OnBnClickedCheckPad20();
	afx_msg void OnBnClickedCheckPad21();
	afx_msg void OnBnClickedCheckPad22();
	afx_msg void OnBnClickedCheckPad23();
	afx_msg void OnBnClickedCheckPad24();
	afx_msg void OnBnClickedCheckPad25();
	afx_msg void OnBnClickedCheckPad26();
	afx_msg void OnBnClickedCheckPad27();
	afx_msg void OnBnClickedCheckPad28();

public:
	// コントロール変数
	CButton m_check_gamepad_use;
	CComboBox m_combo_gamepad_num;
	CEdit m_edit_pad_connected;
	CEdit m_edit_pad_test;

	// パッドテスト
	UINT m_padTimerID;
	DWORD m_padID;
	CString m_connectedPad;
	CString m_padTest;

public:
	void SetValid(bool valid);
	void StartTimer();
	void EndTimer();

public:
	// ユーティリティ
	static void Str2Num(std::vector<WORD>& NUM, CString str)
	{
		str.Replace(L" ", L"");
				
		for (;;)
		{
			int pos = str.Find(L"+");
			if (pos < 0)
			{
				NUM.push_back(_ttoi(str));
				break;
			}
			NUM.push_back(_ttoi(str.Left(pos)));
			str.Delete(0, pos + 1);
		}
	}

	static CString Num2Str(std::vector<WORD> NUM)
	{
		CString str = L"";

		if (NUM.size() == 0)
		{
			str = L"0";
		}
		else
		{
			CString last;
			last.Format(L"%d", NUM.back());
			NUM.pop_back();

			for (size_t i = 0; i < NUM.size(); i++)
			{
				CString n;
				n.Format(L"%d+", NUM[i]);
				str += n;
			}
			str += last;
		}
		return str;
	}	
};
