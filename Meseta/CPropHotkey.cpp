// CPropHotkey.cpp : 実装ファイル
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
	DDX_Control(pDX, IDC_HK_SHIHT, m_rec_shift);
	DDX_Control(pDX, IDC_HK_CTRL, m_rec_ctrl);
	DDX_Control(pDX, IDC_HK_ALT, m_rec_alt);
	DDX_Control(pDX, IDC_HK_WIN, m_rec_win);
	DDX_Control(pDX, IDC_COMBO_HOTKEY, m_comb_vk);
}


BEGIN_MESSAGE_MAP(CPropHotkey, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_HK_SHIHT, &CPropHotkey::OnBnClickedHkShiht)
	ON_BN_CLICKED(IDC_HK_CTRL, &CPropHotkey::OnBnClickedHkCtrl)
	ON_BN_CLICKED(IDC_HK_ALT, &CPropHotkey::OnBnClickedHkAlt)
	ON_BN_CLICKED(IDC_HK_WIN, &CPropHotkey::OnBnClickedHkWin)
	ON_CBN_SELCHANGE(IDC_COMBO_HOTKEY, &CPropHotkey::OnCbnSelchangeComboHotkey)
END_MESSAGE_MAP()


// CPropHotkey メッセージ ハンドラー


BOOL CPropHotkey::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO: ここに初期化を追加してください
	
	CString rec = parentDlg->iniData.hotkeyRec;
	rec = rec.MakeUpper();
	m_rec_shift.SetCheck(rec.Find(L"SHIFT") != -1);
	m_rec_ctrl.SetCheck(rec.Find(L"CTRL") != -1);
	m_rec_alt.SetCheck(rec.Find(L"ALT") != -1);
	m_rec_win.SetCheck(rec.Find(L"WIN") != -1);

	UINT mod, vk;
	CMesetaDlg::Str2VK(rec, mod, vk);
	CString V_KEY((wchar_t*) & vk);
	int pos = m_comb_vk.FindStringExact(0, V_KEY);
	if (pos >= 0)
		m_comb_vk.SetCurSel(pos);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


bool CPropHotkey::checkInvalid()
{
	bool invalid = !m_rec_shift.GetCheck() && !m_rec_ctrl.GetCheck() && !m_rec_alt.GetCheck() && !m_rec_win.GetCheck();

	if (invalid)
	{
		MessageBox(L"一つ以上の修飾キーが必要です。", L"エラー");
	}

	return invalid;
}


void CPropHotkey::OnBnClickedHkShiht()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (checkInvalid())
	{
		m_rec_shift.SetCheck(true);
	}

	SetModified();
}

void CPropHotkey::OnBnClickedHkCtrl()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (checkInvalid())
	{
		m_rec_ctrl.SetCheck(true);
	}

	SetModified();
}


void CPropHotkey::OnBnClickedHkAlt()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (checkInvalid())
	{
		m_rec_alt.SetCheck(true);
	}

	SetModified();
}


void CPropHotkey::OnBnClickedHkWin()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (checkInvalid())
	{
		m_rec_win.SetCheck(true);
	}

	SetModified();
}


void CPropHotkey::OnCbnSelchangeComboHotkey()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	SetModified();
}


void CPropHotkey::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CString VK = L"";
	if (m_rec_shift.GetCheck())
		VK += L"Shift+";

	if (m_rec_ctrl.GetCheck())
		VK += L"Ctrl+";

	if (m_rec_alt.GetCheck())
		VK += L"Alt+";

	if (m_rec_win.GetCheck())
		VK += L"Win+";

	CString key;
	m_comb_vk.GetWindowText(key);
	VK += key;
	

	parentDlg->DeleteHotkey();
	parentDlg->iniData.hotkeyRec = VK;
	parentDlg->SetHotkey();

	CMFCPropertyPage::OnOK();
}
