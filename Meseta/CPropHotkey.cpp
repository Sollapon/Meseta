// CPropHotkey.cpp : 実装ファイル
// 
// 設定・ホットキータブ
//

#include "pch.h"
#include "Meseta.h"
#include "afxdialogex.h"
#include "CPropHotkey.h"


// CPropHotkey ダイアログ

IMPLEMENT_DYNAMIC(CPropHotkey, CMFCPropertyPage)

CPropHotkey::CPropHotkey(CMesetaDlg* parent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_HOTKEY)
	, parentDlg(parent)
{

}

CPropHotkey::~CPropHotkey()
{
}

void CPropHotkey::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HK_SHIFT, m_rec_shift);
	DDX_Control(pDX, IDC_HK_CTRL, m_rec_ctrl);
	DDX_Control(pDX, IDC_HK_ALT, m_rec_alt);
	DDX_Control(pDX, IDC_HK_WIN, m_rec_win);
	DDX_Control(pDX, IDC_COMBO_HOTKEY, m_comb_vk);
	DDX_Control(pDX, IDC_PROP_CHECK_DEL_USE, m_check_use_del);
	DDX_Control(pDX, IDC_HK_SHIFT2, m_del_shift);
	DDX_Control(pDX, IDC_HK_CTRL2, m_del_ctrl);
	DDX_Control(pDX, IDC_HK_ALT2, m_del_alt);
	DDX_Control(pDX, IDC_HK_WIN2, m_del_win);
	DDX_Control(pDX, IDC_COMBO_HOTKEY2, m_comb_vk_del);
}

BEGIN_MESSAGE_MAP(CPropHotkey, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_HK_SHIFT, &CPropHotkey::OnBnClickedHkShiht)
	ON_BN_CLICKED(IDC_HK_CTRL, &CPropHotkey::OnBnClickedHkCtrl)
	ON_BN_CLICKED(IDC_HK_ALT, &CPropHotkey::OnBnClickedHkAlt)
	ON_BN_CLICKED(IDC_HK_WIN, &CPropHotkey::OnBnClickedHkWin)
	ON_CBN_SELCHANGE(IDC_COMBO_HOTKEY, &CPropHotkey::OnCbnSelchangeComboHotkey)
	ON_BN_CLICKED(IDC_PROP_CHECK_DEL_USE, &CPropHotkey::OnBnClickedPropCheckDelUse)
	ON_BN_CLICKED(IDC_HK_SHIFT2, &CPropHotkey::OnBnClickedHkShift2)
	ON_BN_CLICKED(IDC_HK_CTRL2, &CPropHotkey::OnBnClickedHkCtrl2)
	ON_BN_CLICKED(IDC_HK_ALT2, &CPropHotkey::OnBnClickedHkAlt2)
	ON_BN_CLICKED(IDC_HK_WIN2, &CPropHotkey::OnBnClickedHkWin2)
	ON_CBN_SELCHANGE(IDC_COMBO_HOTKEY2, &CPropHotkey::OnCbnSelchangeComboHotkey2)
END_MESSAGE_MAP()


// CPropHotkey メッセージ ハンドラー

// 初期化
BOOL CPropHotkey::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	const wchar_t* modifier[4] = {
		L"SHIFT",
		L"CTRL",
		L"ALT",
		L"WIN"
	};

	check_button[0].clear();
	check_button[1].clear();

	check_button[0].push_back(&m_rec_shift);
	check_button[0].push_back(&m_rec_ctrl);
	check_button[0].push_back(&m_rec_alt);
	check_button[0].push_back(&m_rec_win);

	check_button[1].push_back(&m_del_shift);
	check_button[1].push_back(&m_del_ctrl);
	check_button[1].push_back(&m_del_alt);
	check_button[1].push_back(&m_del_win);

	combo_ctrl.push_back(&m_comb_vk);
	combo_ctrl.push_back(&m_comb_vk_del);


	// INIのホットキー設定を読み取ってチェック
	
	// 記録キー
	{
		CString rec = parentDlg->iniData.hotkeyRec;
		rec = rec.MakeUpper();
		for (size_t i = 0; i < check_button[0].size(); i++)
		{
			check_button[0][i]->SetCheck(rec.Find(modifier[i]) != -1);
		}
		UINT mod, vk;
		CMesetaDlg::Str2VK(rec, mod, vk);
		CString V_KEY((wchar_t*)&vk);
		int pos = m_comb_vk.FindStringExact(0, V_KEY);
		if (pos >= 0)
			m_comb_vk.SetCurSel(pos);
	}

	// 削除キー
	m_check_use_del.SetCheck(parentDlg->iniData.hotkeyUseDell);
	useHotkeyDel(m_check_use_del.GetCheck());
	{
		CString del = parentDlg->iniData.hotkeyDel;
		del = del.MakeUpper();
		for (size_t i = 0; i < check_button[1].size(); i++)
		{
			check_button[1][i]->SetCheck(del.Find(modifier[i]) != -1);
		}
		UINT mod, vk;
		CMesetaDlg::Str2VK(del, mod, vk);
		CString V_KEY((wchar_t*)&vk);
		int pos = m_comb_vk_del.FindStringExact(0, V_KEY);
		if (pos >= 0)
			m_comb_vk_del.SetCurSel(pos);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// 記録キーの修飾キー設定 ////////////////////////////////////////////////////////////
void CPropHotkey::OnBnClickedHkShiht()
{
	if (checkInvalid(0))
	{
		m_rec_shift.SetCheck(true);
	}
	SetModified();
}

void CPropHotkey::OnBnClickedHkCtrl()
{
	if (checkInvalid(0))
	{
		m_rec_ctrl.SetCheck(true);
	}
	SetModified();
}

void CPropHotkey::OnBnClickedHkAlt()
{
	if (checkInvalid(0))
	{
		m_rec_alt.SetCheck(true);
	}
	SetModified();
}

void CPropHotkey::OnBnClickedHkWin()
{
	if (checkInvalid(0))
	{
		m_rec_win.SetCheck(true);
	}
	SetModified();
}

void CPropHotkey::OnCbnSelchangeComboHotkey()
{
	SetModified();
}



// 削除キーの修飾キー設定 ////////////////////////////////////////////////////////////
// 削除キーの使用
void CPropHotkey::OnBnClickedPropCheckDelUse()
{
	useHotkeyDel(m_check_use_del.GetCheck());
	SetModified();
}

void CPropHotkey::OnBnClickedHkShift2()
{
	if (checkInvalid(1))
	{
		m_del_shift.SetCheck(true);
	}
	SetModified();
}

void CPropHotkey::OnBnClickedHkCtrl2()
{
	if (checkInvalid(1))
	{
		m_del_ctrl.SetCheck(true);
	}
	SetModified();
}

void CPropHotkey::OnBnClickedHkAlt2()
{
	if (checkInvalid(1))
	{
		m_del_alt.SetCheck(true);
	}
	SetModified();
}

void CPropHotkey::OnBnClickedHkWin2()
{
	if (checkInvalid(1))
	{
		m_del_win.SetCheck(true);
	}
	SetModified();
}

void CPropHotkey::OnCbnSelchangeComboHotkey2()
{
	SetModified();
}
//////////////////////////////////////////////////////////////////////////////////////


// 適用またはOK時にINIデータを更新する
void CPropHotkey::OnOK()
{
	// 削除キーの使用
	parentDlg->iniData.hotkeyUseDell = m_check_use_del.GetCheck();

	// ホットキーの保存と変更
	parentDlg->DeleteHotkey();
	parentDlg->iniData.hotkeyRec = makeHotkeyStr(0);
	parentDlg->iniData.hotkeyDel = makeHotkeyStr(1);
	parentDlg->SetHotkey();

	CMFCPropertyPage::OnOK();
}


// 修飾キーが最低でも一つ選ばれてるかチェック
bool CPropHotkey::checkInvalid(int idx)
{
	BOOL invalid = true;
	for (size_t i = 0; i < check_button[idx].size(); i++)
	{
		invalid &= (check_button[idx][i]->GetCheck() == FALSE);
	}

	if (invalid)
	{
		MessageBox(L"一つ以上の修飾キーが必要です。", L"エラー");
	}

	return invalid;
}

// 削除キーの変更を有効化する
void CPropHotkey::useHotkeyDel(BOOL use)
{
	for (size_t i = 0; i < check_button[1].size(); i++)
	{
		check_button[1][i]->EnableWindow(use);
	}
	m_comb_vk_del.EnableWindow(use);
}

// チェックボックスの状態からホットキー文字列生成
CString CPropHotkey::makeHotkeyStr(int idx)
{
	const wchar_t* modifier[4] = {
		L"Shift+",
		L"Ctrl+",
		L"Alt+",
		L"Win+"
	};

	CString VK = L"";
	for (size_t i=0; i < check_button[idx].size(); i++)
	{
		if ( check_button[idx][i]->GetCheck() )
			VK += CString(modifier[i]);
	}
	CString key;
	combo_ctrl[idx]->GetWindowText(key);
	VK += key;

	return VK;
}
