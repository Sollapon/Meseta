// CPropFunction.cpp : 実装ファイル
// 
// 設定：動作タブ
//

#include "pch.h"
#include "Meseta.h"
#include "afxdialogex.h"
#include "CPropFunction.h"


// CPropFunction ダイアログ

IMPLEMENT_DYNAMIC(CPropFunction, CMFCPropertyPage)

CPropFunction::CPropFunction(CMesetaDlg* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_FUNCTION)
	, parentDlg(pParent)
{

}

CPropFunction::~CPropFunction()
{
}

void CPropFunction::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTO_REFRESH, m_check_auto_refresh);
	DDX_Control(pDX, IDC_COMBO_AUTO_REFRESH, m_combo_auto_refresh);
	DDX_Control(pDX, IDC_CHECK_SAVE_POS, m_check_save_pos);
}

BEGIN_MESSAGE_MAP(CPropFunction, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_AUTO_REFRESH, &CPropFunction::OnBnClickedCheckAutoRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTO_REFRESH, &CPropFunction::OnCbnSelchangeComboAutoRefresh)
	ON_BN_CLICKED(IDC_CHECK_SAVE_POS, &CPropFunction::OnBnClickedCheckSavePos)
	ON_CBN_EDITCHANGE(IDC_COMBO_AUTO_REFRESH, &CPropFunction::OnEditchangeComboAutoRefresh)
END_MESSAGE_MAP()


// CPropFunction メッセージ ハンドラー

// 初期化
BOOL CPropFunction::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// INIデータから初期値をセット
	m_check_auto_refresh.SetCheck(parentDlg->iniData.auto_refresh);
	m_check_save_pos.SetCheck(parentDlg->iniData.pos_save);

	// 自動更新時間の文字列セット
	CString time;
	time.Format(L"%d", parentDlg->iniData.refresh_second);
	m_combo_auto_refresh.SetWindowText(time);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// 適用またはOK時にINIデータを更新する
void CPropFunction::OnOK()
{
	// INI更新
	parentDlg->iniData.auto_refresh = m_check_auto_refresh.GetCheck();
	parentDlg->iniData.pos_save = m_check_save_pos.GetCheck();

	CString time;
	m_combo_auto_refresh.GetWindowText(time);
	int int_time = _ttoi(time);
	if (int_time <= 0) // コンボコントロールなので不正な値が入る可能性がある
	{
		int_time = 10;
		m_combo_auto_refresh.SetWindowText(L"10");
	}
	parentDlg->iniData.refresh_second = int_time;

	CMFCPropertyPage::OnOK();
}

// チェックボックスをクリックしたら変更フラグ立てる
void CPropFunction::OnBnClickedCheckAutoRefresh()
{
	SetModified();
}

// チェックボックスをクリックしたら変更フラグ立てる
void CPropFunction::OnCbnSelchangeComboAutoRefresh()
{
	SetModified();
}

// チェックボックスをクリックしたら変更フラグ立てる
void CPropFunction::OnBnClickedCheckSavePos()
{
	SetModified();
}

// コンボコントロールの文字列が変更された
void CPropFunction::OnEditchangeComboAutoRefresh()
{
	SetModified();
}

// 適用時のリライト
BOOL CPropFunction::OnSetActive()
{
	if (parentDlg->rewriteStatus)
	{
		parentDlg->openStatusWindow();
		parentDlg->rewriteStatus = FALSE;
	}

	return CMFCPropertyPage::OnSetActive();
}
