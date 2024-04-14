// CPropDisp.cpp : 実装ファイル
// 
// 設定：外観
//

#include "pch.h"
#include "Meseta.h"
#include "afxdialogex.h"
#include "CPropDisp.h"


// CPropDisp ダイアログ

IMPLEMENT_DYNAMIC(CPropDisp, CMFCPropertyPage)

CPropDisp::CPropDisp(CMesetaDlg* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_DISP)
	, parentDlg(pParent)
{

}

CPropDisp::~CPropDisp()
{
}

void CPropDisp::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SAVE_POS, m_check_save_pos);
	DDX_Control(pDX, IDC_COMBO_LOG_NUM, m_combo_log_num);
	DDX_Control(pDX, IDC_COMBO_SUB_DATA, m_combo_sub_data);
}


BEGIN_MESSAGE_MAP(CPropDisp, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_LOG_NUM, &CPropDisp::OnCbnSelchangeComboLogNum)
	ON_CBN_SELCHANGE(IDC_COMBO_SUB_DATA, &CPropDisp::OnCbnSelchangeComboSubData)
	ON_BN_CLICKED(IDC_CHECK_SAVE_POS, &CPropDisp::OnBnClickedCheckSavePos)
END_MESSAGE_MAP()


// CPropDisp メッセージ ハンドラー

// 初期化処理
BOOL CPropDisp::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// INIデータから初期値をセット
	m_check_save_pos.SetCheck(parentDlg->iniData.pos_save);
	m_combo_log_num.SetCurSel(parentDlg->iniData.disp_logNum - 1);
	m_combo_sub_data.SetCurSel(int(parentDlg->iniData.disp_interval == TRUE));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// 適用またはOK時にINIデータを更新する
void CPropDisp::OnOK()
{
	// INI更新
	parentDlg->iniData.pos_save = m_check_save_pos.GetCheck();
	parentDlg->iniData.disp_interval = (m_combo_sub_data.GetCurSel() == 1);

	// ステータスウィンドウ再描画
	if ((parentDlg->iniData.disp_logNum - 1) != m_combo_log_num.GetCurSel())
	{
		parentDlg->rewriteStatus = TRUE;
		parentDlg->iniData.disp_logNum = m_combo_log_num.GetCurSel() + 1;
	}

	CMFCPropertyPage::OnOK();
}

// 適用時のリライト
BOOL CPropDisp::OnSetActive()
{
	if (parentDlg->rewriteStatus)
	{
		parentDlg->openStatusWindow();
		parentDlg->rewriteStatus = FALSE;
	}

	return CMFCPropertyPage::OnSetActive();
}

// 直近ログの数変更
void CPropDisp::OnCbnSelchangeComboLogNum()
{
	SetModified();
}

// サブデータの変更
void CPropDisp::OnCbnSelchangeComboSubData()
{
	SetModified();
}

// ウィンドウ位置の保存
void CPropDisp::OnBnClickedCheckSavePos()
{
	SetModified();
}
