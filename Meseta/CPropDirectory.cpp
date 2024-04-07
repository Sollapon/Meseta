// CPropDirectory.cpp : 実装ファイル
// 
// 設定・動作ディレクトリ
//

#include "pch.h"
#include "Meseta.h"
#include "afxdialogex.h"
#include "CPropDirectory.h"


// CPropDirectory ダイアログ

IMPLEMENT_DYNAMIC(CPropDirectory, CMFCPropertyPage)

CPropDirectory::CPropDirectory(CMesetaDlg* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_DIRECTORY)
	, parentDlg(pParent)
{

}

CPropDirectory::~CPropDirectory()
{
}

void CPropDirectory::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG_NGS, m_edit_log_ngs);
	DDX_Control(pDX, IDC_EDIT_DIR_HELP, m_edit_directory);
}


BEGIN_MESSAGE_MAP(CPropDirectory, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_LOG_NGS, &CPropDirectory::OnEnChangeEditLogNgs)
	ON_BN_CLICKED(IDC_BUTTON_LOG_NGS, &CPropDirectory::OnBnClickedButtonLogNgs)
END_MESSAGE_MAP()


// CPropDirectory メッセージ ハンドラー

// 初期化
BOOL CPropDirectory::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// INIからパス読み取り
	m_edit_log_ngs.SetWindowText(parentDlg->iniData.ngs_log_path);

	// ヘルプ
	m_edit_directory.SetWindowText(L"NGSのアクションログファイルは通常\r\n%UserProfile%\\Documents\\SEGA\r\nもしくは\r\n%OneDrive%\\ドキュメント\\SEGA\r\n以下にある\r\n\\PHANTASYSTARONLINE2\\log_ngs\r\nに保存されています。\r\nエラーが出る場合は正しいパスを指定してください。");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
	
}

void CPropDirectory::OnOK()
{
	// 指定されたパスをチェック
	CString path;
	m_edit_log_ngs.GetWindowText(path);
	bool ret = parentDlg->mesetaCtrl.Init(path, false);

	if (ret)
	{
		parentDlg->iniData.ngs_log_path = path;
	}
	else
	{
		// パス指定が不正
		MessageBox(L"ログファイルが見つかりませんでした。\n正しいパスを設定してください。", L"エラー");
		SetModified();
	}

	CMFCPropertyPage::OnOK();
}

// ディレクトリのパスが変更された
void CPropDirectory::OnEnChangeEditLogNgs()
{
	SetModified();
}

// ディレクトリ変更ボタン
void CPropDirectory::OnBnClickedButtonLogNgs()
{
	// ファイル検索ダイアログ
	CFolderPickerDialog folderDialog;

	// ダイアログ表示
	if (folderDialog.DoModal() == IDOK)
	{
		// [フォルダーの選択]ボタン押下
		// フルパス取得
		CString fullPathName = folderDialog.GetPathName();

		// ここでfullPathNameに格納されたフルパスを使用する
		m_edit_log_ngs.SetWindowText(fullPathName);
	}
}
