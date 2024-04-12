// CPropGamepad.cpp : 実装ファイル
// 
// 設定：ゲームパッド
//

#include "pch.h"
#include "Meseta.h"
#include "afxdialogex.h"
#include "CPropGamepad.h"


// CPropGamepad ダイアログ

IMPLEMENT_DYNAMIC(CPropGamepad, CMFCPropertyPage)

CPropGamepad::CPropGamepad(CMesetaDlg* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_GAMEPAD)
	, parentDlg(pParent)
{

}

CPropGamepad::~CPropGamepad()
{
}

void CPropGamepad::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKGAMEPAD_USE, m_check_gamepad_use);
	DDX_Control(pDX, IDC_COMBO_GAMEPAD_ID, m_combo_gamepad_num);
}


BEGIN_MESSAGE_MAP(CPropGamepad, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_CHECKGAMEPAD_USE, &CPropGamepad::OnBnClickedCheckgamepadUse)
	ON_CBN_SELCHANGE(IDC_COMBO_GAMEPAD_ID, &CPropGamepad::OnCbnSelchangeComboGamepadId)
	ON_BN_CLICKED(IDC_CHECK_PAD1, &CPropGamepad::OnBnClickedCheckPad1)
	ON_BN_CLICKED(IDC_CHECK_PAD2, &CPropGamepad::OnBnClickedCheckPad2)
	ON_BN_CLICKED(IDC_CHECK_PAD3, &CPropGamepad::OnBnClickedCheckPad3)
	ON_BN_CLICKED(IDC_CHECK_PAD4, &CPropGamepad::OnBnClickedCheckPad4)
	ON_BN_CLICKED(IDC_CHECK_PAD5, &CPropGamepad::OnBnClickedCheckPad5)
	ON_BN_CLICKED(IDC_CHECK_PAD6, &CPropGamepad::OnBnClickedCheckPad6)
	ON_BN_CLICKED(IDC_CHECK_PAD7, &CPropGamepad::OnBnClickedCheckPad7)
	ON_BN_CLICKED(IDC_CHECK_PAD8, &CPropGamepad::OnBnClickedCheckPad8)
	ON_BN_CLICKED(IDC_CHECK_PAD9, &CPropGamepad::OnBnClickedCheckPad9)
	ON_BN_CLICKED(IDC_CHECK_PAD10, &CPropGamepad::OnBnClickedCheckPad10)
	ON_BN_CLICKED(IDC_CHECK_PAD11, &CPropGamepad::OnBnClickedCheckPad11)
	ON_BN_CLICKED(IDC_CHECK_PAD12, &CPropGamepad::OnBnClickedCheckPad12)
	ON_BN_CLICKED(IDC_CHECK_PAD13, &CPropGamepad::OnBnClickedCheckPad13)
	ON_BN_CLICKED(IDC_CHECK_PAD14, &CPropGamepad::OnBnClickedCheckPad14)
	ON_BN_CLICKED(IDC_CHECK_PAD15, &CPropGamepad::OnBnClickedCheckPad15)
	ON_BN_CLICKED(IDC_CHECK_PAD16, &CPropGamepad::OnBnClickedCheckPad16)
	ON_BN_CLICKED(IDC_CHECK_PAD17, &CPropGamepad::OnBnClickedCheckPad17)
	ON_BN_CLICKED(IDC_CHECK_PAD18, &CPropGamepad::OnBnClickedCheckPad18)
	ON_BN_CLICKED(IDC_CHECK_PAD19, &CPropGamepad::OnBnClickedCheckPad19)
	ON_BN_CLICKED(IDC_CHECK_PAD20, &CPropGamepad::OnBnClickedCheckPad20)
	ON_BN_CLICKED(IDC_CHECK_PAD21, &CPropGamepad::OnBnClickedCheckPad21)
	ON_BN_CLICKED(IDC_CHECK_PAD22, &CPropGamepad::OnBnClickedCheckPad22)
	ON_BN_CLICKED(IDC_CHECK_PAD23, &CPropGamepad::OnBnClickedCheckPad23)
	ON_BN_CLICKED(IDC_CHECK_PAD24, &CPropGamepad::OnBnClickedCheckPad24)
	ON_BN_CLICKED(IDC_CHECK_PAD25, &CPropGamepad::OnBnClickedCheckPad25)
	ON_BN_CLICKED(IDC_CHECK_PAD26, &CPropGamepad::OnBnClickedCheckPad26)
	ON_BN_CLICKED(IDC_CHECK_PAD27, &CPropGamepad::OnBnClickedCheckPad27)
	ON_BN_CLICKED(IDC_CHECK_PAD28, &CPropGamepad::OnBnClickedCheckPad28)
END_MESSAGE_MAP()


// CPropGamepad メッセージ ハンドラー

// ページ初期化
BOOL CPropGamepad::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// INIデータから初期値をセット
	m_check_gamepad_use.SetCheck(parentDlg->iniData.padUse);
	SetValid(m_check_gamepad_use.GetCheck());

	CString PadNum;
	PadNum.Format(L"%d", parentDlg->iniData.padNum);
	m_combo_gamepad_num.SelectString(0, PadNum);
	
	// ボタン文字列変換
	std::vector<WORD> NUM;
	Str2Num(NUM, parentDlg->iniData.padRec);
	for (size_t i = 0; i < NUM.size(); i++)
	{
		if (NUM[i] == 0) break;

		CButton* button = (CButton*)GetDlgItem(PAD_IDC[0][NUM[i]]);
		button->SetCheck(true);
	}

	NUM.clear();
	Str2Num(NUM, parentDlg->iniData.padDel);
	for (size_t i = 0; i < NUM.size(); i++)
	{
		if (NUM[i] == 0) break;

		CButton* button = (CButton*)GetDlgItem(PAD_IDC[1][NUM[i]]);
		button->SetCheck(true);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// 適用・OK時の処理
void CPropGamepad::OnOK()
{
	// ゲームパッド使用
	parentDlg->iniData.padUse = m_check_gamepad_use.GetCheck();

	// パッド番号
	CString pn;
	m_combo_gamepad_num.GetWindowText(pn);
	parentDlg->iniData.padNum = _ttoi(pn);

	// ボタン番号
	std::vector<WORD> NUM;
	for (int i = 1; i <= 14; i++)
	{
		CButton* button = (CButton*)GetDlgItem(PAD_IDC[0][i]);
		if (button->GetCheck())
			NUM.push_back(i);
	}
	parentDlg->iniData.padRec = Num2Str(NUM);

	NUM.clear();
	for (int i = 1; i <= 14; i++)
	{
		CButton* button = (CButton*)GetDlgItem(PAD_IDC[1][i]);
		if (button->GetCheck())
			NUM.push_back(i);
	}
	parentDlg->iniData.padDel = Num2Str(NUM);

	// パッドBit
	parentDlg->padInfo.clear();
	parentDlg->padInfo.setBit(0, parentDlg->iniData.padRec);
	parentDlg->padInfo.setBit(1, parentDlg->iniData.padDel);

	CMFCPropertyPage::OnOK();
}

// ゲームパッドの使用設定
void CPropGamepad::OnBnClickedCheckgamepadUse()
{
	SetValid(m_check_gamepad_use.GetCheck());
	SetModified();
}

// パッドID設定
void CPropGamepad::OnCbnSelchangeComboGamepadId()
{
	SetModified();
}


// 変更通知
void CPropGamepad::OnBnClickedCheckPad1()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad2()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad3()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad4()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad5()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad6()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad7()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad8()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad9()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad10()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad11()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad12()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad13()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad14()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad15()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad16()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad17()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad18()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad19()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad20()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad21()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad22()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad23()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad24()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad25()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad26()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad27()
{
	SetModified();
}
void CPropGamepad::OnBnClickedCheckPad28()
{
	SetModified();
}


void CPropGamepad::SetValid(bool valid)
{
	m_combo_gamepad_num.EnableWindow(valid);

	GetDlgItem(IDC_STATIC_PADID)->EnableWindow(valid);

	for (int key = 0; key < 2; key++)
	{
		for (int i = 1; i <= 14; i++)
		{
			GetDlgItem(PAD_IDC[key][i])->EnableWindow(valid);
		}
	}
}