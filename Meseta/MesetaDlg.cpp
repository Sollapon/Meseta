
// MesetaDlg.cpp : 実装ファイル
//

//#include "pch.h"
#include "framework.h"
#include "afxdialogex.h"

#include "Meseta.h"
#include "MesetaDlg.h"

#include "CPropHotkey.h"
#include "CPropDirectory.h"
#include "CPropFunction.h"


// Xinputライブラリ
#pragma comment(lib, "xinput9_1_0.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// NGSディレクトリ初期設定
#define DEFAULT_DOC L"%UserProfile%\\Documents"
#define DEFAULT_NGS_LOG L"\\SEGA\\PHANTASYSTARONLINE2\\log_ngs\\"


// CMesetaDlg ダイアログ
CMesetaDlg::CMesetaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MESETA_DIALOG, pParent)
	, m_statusWindow(NULL)
	, m_timerID(0)
	, m_autoFinishTimerID(0)
	, m_padTimerID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMesetaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_CHECK_PRE_ST, m_check_pre_status);
	DDX_Control(pDX, IDC_CHECK_PRE_LOG, m_check_pre_log);
	DDX_Control(pDX, IDC_CHECK_TRA_LOG, m_check_trans_log);
	DDX_Control(pDX, IDC_SLIDER1, m_window_alpha);
	DDX_Control(pDX, IDC_CHECK_AUTO, m_check_use_auto_finish);
	DDX_Control(pDX, IDC_COMBO1, m_auto_finish_count);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON_END, m_buttonEnd);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_buttonConfig);
}

BEGIN_MESSAGE_MAP(CMesetaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_HOTKEY()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CMesetaDlg::OnCustomdrawList1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_PRE_ST, &CMesetaDlg::OnBnClickedCheckPreSt)
	ON_BN_CLICKED(IDC_CHECK_PRE_LOG, &CMesetaDlg::OnBnClickedCheckPreLog)
	ON_BN_CLICKED(IDC_CHECK_TRA_LOG, &CMesetaDlg::OnBnClickedCheckTraLog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CMesetaDlg::OnBnClickedCheckAuto)
	ON_BN_CLICKED(IDC_BUTTON_END, &CMesetaDlg::OnBnClickedButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_START, &CMesetaDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CMesetaDlg::OnBnClickedButtonConfig)
END_MESSAGE_MAP()


// CMesetaDlg メッセージ ハンドラー
BOOL CMesetaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// リストコントロールのスタイル変更
	DWORD dwStyleEx = m_listCtrl.GetExtendedStyle();
	dwStyleEx |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_listCtrl.SetExtendedStyle(dwStyleEx);

	// リストコントロールの列タイトル
	m_listCtrl.InsertColumn(0, _T("id"), LVCFMT_CENTER, 25);
	m_listCtrl.InsertColumn(1, _T("メセタ"), LVCFMT_CENTER, 90);
	m_listCtrl.InsertColumn(2, _T("開始時刻"), LVCFMT_CENTER, 90);
	m_listCtrl.InsertColumn(3, _T("記録時間"), LVCFMT_CENTER, 70);
	m_listCtrl.InsertColumn(4, _T("メセタ/s"), LVCFMT_CENTER, 80);
	m_listCtrl.InsertColumn(5, _T("インターバル"), LVCFMT_CENTER, 70);

	// 終了ボタンは初期値無効化
	m_buttonEnd.EnableWindow(false);

	// ウィンドウの拡張スタイルを変更
	ModifyStyleEx(0, WS_EX_LAYERED);
	// レイヤードウィンドウの不透明度と透明のカラーキー
	SetLayeredWindowAttributes(0, 255, LWA_ALPHA);	

	// サブウィンドウ作成
	m_statusWindow = new CStatusWindow(this);
	m_statusWindow->Create(IDD_FORMVIEW, this);
	
	// ステータスウィンドウ初期化
	clearStatusWindow();

	// INIファイルが存在するかチェック
	checkIniFile();

	// INIファイルの読み込み
	readINI();

	// サブカラー設定	
	// INIの設定を読み込んだ後に
	m_statusWindow->SetColor(iniData.dlgColor, iniData.fntColorN);

	// ホットキー設定
	// INIの設定を読み込んだ後に
	SetHotkey();

	// パッド情報初期化
	// INIの設定を読み込んだ後に
	padInfo.clear();
	padInfo.setBit(0, iniData.padRec);
	padInfo.setBit(1, iniData.padDel);
	
	// 初期化（実行無し)
	Init(false);	
	
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ホットキーの設定
// INIファイルの読み込み結果の繁栄
bool CMesetaDlg::SetHotkey()
{
	// 記録キーの設定
	UINT mod, vk;
	CMesetaDlg::Str2VK(iniData.hotkeyRec, mod, vk);
	BOOL ret1 = ::RegisterHotKey(GetSafeHwnd(), 100, mod, vk);
	BOOL ret2 = true;
	CString err = L"ホットキーが既に別のアプリで使用されています。\n設定から使用可能なキーを割り当ててください。\n";
	if (!ret1)
	{
		CString key = iniData.hotkeyRec;
		MessageBox(err+L"("+key + L")", L"エラー"); // 既に登録済
	}

	// 削除キーの設定
	if (iniData.hotkeyUseDell)
	{
		CMesetaDlg::Str2VK(iniData.hotkeyDel, mod, vk);
		ret2 = ::RegisterHotKey(GetSafeHwnd(), 101, mod, vk);
		if (!ret2)
		{
			CString key = iniData.hotkeyDel;
			MessageBox(err + L"(" + key + L")", L"エラー");// 既に登録済
		}
	}

	return (ret1&ret2);
}


// ホットキーの削除
bool CMesetaDlg::DeleteHotkey()
{
	BOOL ret1 = ::UnregisterHotKey(GetSafeHwnd(), 100);
	BOOL ret2 = ::UnregisterHotKey(GetSafeHwnd(), 101);

	return (ret1&ret2);
}


// ステータス表示の初期化
void CMesetaDlg::clearStatusWindow()
{
	if (m_statusWindow)
	{
		// 表示を全て初期化する
		m_statusWindow->SetTimeCount(0, 0, 0);
		m_statusWindow->SetTotalMeseta(0, 0);
		m_statusWindow->clearLog();
	}
}


// INIファイルが存在するかチェックする
// 無い場合は作る
bool CMesetaDlg::checkIniFile()
{
	CFileFind find;
	CString filePath = _T("Meseta.ini");
	bool readReg = false;

	// INIファイル有り
	if (find.FindFile(filePath))
	{ 
		CWinApp* pApp = AfxGetApp();
		CString path = pApp->GetProfileString(L"LOG_DIR", L"MGS_LOG", L"TEST");

		//　ファイルが存在してても中身が空の場合は読み取れない
		if (path == L"TEST")
			readReg = true;
		
	}
	// INIファイル無し
	else
	{
		readReg = true;
	}

	// 存在しないか空の場合はUTF16で作る
	if (readReg )
	{
		FILE* fp;
		_wfopen_s(& fp, filePath, _T("w, ccs=UTF-16LE"));
		CStdioFile file(fp);
		if (!file)
			return false;
		CString path = L"ここに正しいngs_logフォルダのパスを設定してください。";

		// ログファイルは標準ではカレントユーザーのDocumentsに作られるが
		// OneDriveを使ってる人の場合はOneDriveのドキュメント以下に作られる
		// レジストリからドキュメントフォルダがどっちに作られてるか読み取る
		HKEY hKye;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), NULL, KEY_QUERY_VALUE, &hKye) == ERROR_SUCCESS)
		{			
			DWORD dwType;
			TCHAR lpData[1024];
			DWORD dwDataSize;
			dwDataSize = sizeof(lpData) / sizeof(lpData[0]);
			LSTATUS err = RegQueryValueEx(hKye, TEXT("Personal"), 0, &dwType, (LPBYTE)lpData , &dwDataSize);
			RegCloseKey(hKye);

			// ngs_logファイルがあると思われる場所
			path = CString(L"MGS_LOG=") + CString(lpData) + CString(DEFAULT_NGS_LOG) + CString(L"\n");
		}

		file.WriteString(L"[LOG_DIR]\n");
		file.WriteString(path);
		file.Close();
	}
	return true;
}

// INIファイルの読み取り
BOOL CMesetaDlg::readINI()
{
	// NGS_LOGフォルダへのパスを読み込み
	CWinApp* pApp = AfxGetApp();
	iniData.ngs_log_path = pApp->GetProfileString(L"LOG_DIR", L"MGS_LOG", CString(DEFAULT_DOC)+CString(DEFAULT_NGS_LOG));

	// 正しいフォルダが設定されてるかチェックする
	BOOL ret = mesetaCtrl.Init(iniData.ngs_log_path);

	if (!ret)
	{
		MessageBox(L"NGSログファイルが読み取れません。\n設定から正しいフォルダを指定してください。", L"エラー");
	}
	
	// ダイアログの初期位置
	CString ini;
	ini = pApp->GetProfileString(L"DLG_POS", L"SAVE", L"FALSE");
	int Dlg_top = pApp->GetProfileInt(L"DLG_POS", L"TOP", 0);
	int Dlg_left = pApp->GetProfileInt(L"DLG_POS", L"LEFT", 0);
	int Sub_Dlg_top = pApp->GetProfileInt(L"DLG_POS", L"SUB_TOP", 0);
	int Sub_Dlg_left = pApp->GetProfileInt(L"DLG_POS", L"SUB_LEFT", 0);
	if (ini == L"FALSE")
	{
		Sub_Dlg_top = Dlg_top;
		Sub_Dlg_left = Dlg_left + 485;
		iniData.pos_save = false;
	}
	else
		iniData.pos_save = true;

	// 最前面設定
	ini = pApp->GetProfileString(L"TOP_WINDOW", L"LOG", L"FALSE");
	m_check_pre_log.SetCheck(ini == L"TRUE");

	ini = pApp->GetProfileString(L"TOP_WINDOW", L"STATUS", L"TRUE");
	m_check_pre_status.SetCheck((ini == L"TRUE") | (bool)m_check_pre_log.GetCheck());

	bool check1 = m_check_pre_log.GetCheck();
	SetWindowPos(check1 ? &CMesetaDlg::wndTopMost : &CMesetaDlg::wndNoTopMost, Dlg_left, Dlg_top, 0, 0, (SWP_NOSIZE | SWP_SHOWWINDOW));
	if (check1)
		m_check_pre_status.EnableWindow(false);

	bool check2 = m_check_pre_status.GetCheck();
	m_statusWindow->setTop(check1||check2, Sub_Dlg_top, Sub_Dlg_left);

	// 透明度
	m_window_alpha.SetRange(32, 255);
	int pos = pApp->GetProfileInt(L"TRANSPARENT", L"ALPHA", 144);
	m_window_alpha.SetPos(pos);
	m_statusWindow->SetAlpha(pos);
	ini = pApp->GetProfileString(L"TRANSPARENT", L"TRANS_LOG", L"FALSE");
	m_check_trans_log.SetCheck((ini == L"TRUE"));
	if(m_check_trans_log.GetCheck())
		SetLayeredWindowAttributes(0, pos, LWA_ALPHA);

	// 自動終了
	ini = pApp->GetProfileString(L"AUTO_FINISH", L"USE", L"FALSE");
	m_check_use_auto_finish.SetCheck(ini == L"TRUE");
	if (!m_check_use_auto_finish.GetCheck())
	{
		m_auto_finish_count.EnableWindow(false);
	}
	int count = pApp->GetProfileInt(L"AUTO_FINISH", L"SECOND", 100);
	ini.Format(L"%d", count);
	m_auto_finish_count.SetWindowText(ini);
	
	// 自動更新
	ini = pApp->GetProfileString(L"AUTO_REFRESH", L"USE", L"TRUE");
	iniData.auto_refresh = (ini == L"TRUE");
	iniData.refresh_second = pApp->GetProfileInt(L"AUTO_REFRESH", L"SECOND", 15);

	// インターバル表示
	ini = pApp->GetProfileString(L"DISP", L"INTERVAL", L"FALSE");
	iniData.disp_interval = (ini == L"TRUE");

	// 色読み取り
	ini = pApp->GetProfileString(L"COLORE", L"DIALOG", L"000000");
	iniData.dlgColor = CMesetaDlg::Str2Col(ini);

	ini = pApp->GetProfileString(L"COLORE", L"FONT_N", L"FFFFFF");
	iniData.fntColorN = CMesetaDlg::Str2Col(ini);

	ini = pApp->GetProfileString(L"COLORE", L"FONT_R", L"FF8000");
	iniData.fntColorR = CMesetaDlg::Str2Col(ini);

	// ホットキー読み取り
	ini = pApp->GetProfileString(L"HOTKEY", L"USE_DELL", L"FALSE");
	iniData.hotkeyUseDell = (ini == L"TRUE");
	iniData.hotkeyRec = pApp->GetProfileString(L"HOTKEY", L"REC", L"Ctrl+Shift+S");
	iniData.hotkeyDel = pApp->GetProfileString(L"HOTKEY", L"DEL", L"Ctrl+Shift+D");

	// ゲームパッド
	ini = pApp->GetProfileString(L"GAMEPAD", L"USE", L"FALSE");
	iniData.padUse = (ini == L"TRUE");
	iniData.padRec = pApp->GetProfileString(L"GAMEPAD", L"REC", L"2+5");
	iniData.padDel = pApp->GetProfileString(L"GAMEPAD", L"DEL", L"1+3+5");
	iniData.padNum = pApp->GetProfileInt(L"GAMEPAD", L"NUM", 0);
	
	return ret;
}

// INIファイルの書き込み
BOOL CMesetaDlg::writeINI()
{
	RECT r;
	GetWindowRect(&r);
	CWinApp* pApp = AfxGetApp();

	// ダイアログの位置
	pApp->WriteProfileInt(L"DLG_POS", L"TOP", r.top);
	pApp->WriteProfileInt(L"DLG_POS", L"LEFT", r.left);

	// ログのパス
	pApp->WriteProfileString(L"LOG_DIR", L"MGS_LOG", iniData.ngs_log_path);

	// ウィンドウ位置の保存
	pApp->WriteProfileString(L"DLG_POS", L"SAVE", iniData.pos_save?L"TRUE":L"FALSE");

	// インターバル表示に変更
	pApp->WriteProfileString(L"DISP", L"INTERVAL", iniData.disp_interval ? L"TRUE" : L"FALSE");

	// 最前面設定
	pApp->WriteProfileString(L"TOP_WINDOW", L"LOG", (bool)m_check_pre_log.GetCheck()?L"TRUE":L"FALSE");
	pApp->WriteProfileString(L"TOP_WINDOW", L"STATUS", (bool)m_check_pre_status.GetCheck() ? L"TRUE" : L"FALSE");

	// 透明度
	pApp->WriteProfileInt(L"TRANSPARENT", L"ALPHA", m_window_alpha.GetPos());
	pApp->WriteProfileString(L"TRANSPARENT", L"TRANS_LOG", (bool)m_check_trans_log.GetCheck() ? L"TRUE" : L"FALSE");

	// 自動終了
	pApp->WriteProfileString(L"AUTO_FINISH", L"USE", (bool)m_check_use_auto_finish.GetCheck() ? L"TRUE" : L"FALSE");
	CString ini;
	m_auto_finish_count.GetWindowText(ini);
	pApp->WriteProfileInt(L"AUTO_FINISH", L"SECOND", _ttoi(ini));

	// 自動更新
	pApp->WriteProfileString(L"AUTO_REFRESH", L"USE", iniData.auto_refresh ? L"TRUE" : L"FALSE");
	pApp->WriteProfileInt(L"AUTO_REFRESH", L"SECOND", iniData.refresh_second);

	// 色
	pApp->WriteProfileString(L"COLORE", L"DIALOG", CMesetaDlg::Col2Str(iniData.dlgColor));
	pApp->WriteProfileString(L"COLORE", L"FONT_N", CMesetaDlg::Col2Str(iniData.fntColorN));
	pApp->WriteProfileString(L"COLORE", L"FONT_R", CMesetaDlg::Col2Str(iniData.fntColorR));

	// ホットキー
	pApp->WriteProfileString(L"HOTKEY", L"USE_DELL", (bool)iniData.hotkeyUseDell ? L"TRUE" : L"FALSE");
	pApp->WriteProfileString(L"HOTKEY", L"REC", iniData.hotkeyRec);
	pApp->WriteProfileString(L"HOTKEY", L"DEL", iniData.hotkeyDel);

	// ゲームパッド
	pApp->WriteProfileString(L"GAMEPAD", L"USE", (bool)iniData.padUse ? L"TRUE" : L"FALSE");
	pApp->WriteProfileString(L"GAMEPAD", L"REC", iniData.padRec);
	pApp->WriteProfileString(L"GAMEPAD", L"DEL", iniData.padDel);
	pApp->WriteProfileInt(L"GAMEPAD", L"NUM", iniData.padNum);

	return true;
}

// 初期化処理
// bool run : 起動有りならture、起動無しならfalse
void CMesetaDlg::Init(bool run)
{
	// データ管理クラスを初期化
	bool ret = mesetaCtrl.Init(iniData.ngs_log_path, run);

	// リストコントロール初期化
	m_listCtrl.DeleteAllItems();

	// サブウィンドウ初期化
	clearStatusWindow();

	// 起動有
	if (run)
	{
		// 経過時間タイマー始動
		if (m_timerID == 0)
		{
			m_timerID = 1;
			SetTimer(m_timerID, 1000, NULL);
		}

		// ゲームパッドの読み取り開始
		if (iniData.padUse && m_padTimerID == 0)
		{
			m_padTimerID = 105;
			SetTimer(m_padTimerID, 16, NULL);
		}

		// ステータスウィンドウ更新
		CString info;
		info.Format(L"%s\n%s", L"記録開始", iniData.hotkeyRec.GetString());
		m_statusWindow->SetInfo(info);
	}
	else
	{
		// 起動無しの場合
		// アプリ起動時と設定ボタンを押したとき
		m_statusWindow->SetInfo(L"スタートで起動");
	}
}

// 記録終了時の一括処理
// currentMeseta=-1 : ログに追加せずに強制終了
void CMesetaDlg::finishRecData(long long currentMeseta)
{
	// 最終結果取得
	mesetaCtrl.endCurrentData(currentMeseta);

	// 正常終了処理の場合はログに追加
	if (currentMeseta >= 0)
	{
		// 計測結果をログのリストに積む
		MesetaData md = mesetaCtrl.pushCurrentData();

		// リストコントロールに追加
		CString addText;
		addText.Format(L"%d", md.idx);
		m_listCtrl.InsertItem(0, addText);

		addText = MesetaDataCtrl::MakeBigNumber(md.meseta);
		m_listCtrl.SetItemText(0, 1, addText);

		addText = md.start.Format(L"%m/%d %H:%M:%S");
		m_listCtrl.SetItemText(0, 2, addText);

		addText.Format(L"%lld秒", md.recTime);
		m_listCtrl.SetItemText(0, 3, addText);

		addText = MesetaDataCtrl::MakeBigNumber(md.mps);
		addText += L"/s";
		m_listCtrl.SetItemText(0, 4, addText);

		m_listCtrl.SetItemText(0, 5, MesetaDataCtrl::Time2Min(md.interval));
	}

	// 自動終了処理の変更を有効化
	m_check_use_auto_finish.EnableWindow(true);
	if (m_check_use_auto_finish.GetCheck())
		m_auto_finish_count.EnableWindow(true);

	// ステータスウィンドウ更新
	CTime now = CTime::GetCurrentTime();
	m_statusWindow->SetTimeCount(mesetaCtrl.getElapsedTime(now));
	m_statusWindow->SetColor(iniData.dlgColor, iniData.fntColorN);
	m_statusWindow->SetTotalMeseta(mesetaCtrl.increasedMeseta, mesetaCtrl.mesetaPerHour);
	m_statusWindow->SetLog(mesetaCtrl.mesetaData, iniData.disp_interval);
	CString info;
	info.Format(L"%s\n%s", L"記録開始", iniData.hotkeyRec.GetString());
	m_statusWindow->SetInfo(info);

	// 自動終了のタイマーが動いてたら殺す
	if (m_autoFinishTimerID != 0)
	{
		KillTimer(m_autoFinishTimerID);
		m_autoFinishTimerID = 0;
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。
void CMesetaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMesetaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// メッセージ処理のオーバーライド
BOOL CMesetaDlg::PreTranslateMessage(MSG* pMsg)
{
	// ダイアログのEnterとEscの規定動作を殺す
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam) {
			return TRUE;
		}
		else if (VK_ESCAPE == pMsg->wParam) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// ホットキーの処理
void CMesetaDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// 起動中以外は無効化
	if (!mesetaCtrl.isRunning())
	{
	}
	// 記録キー
	else if (nHotKeyId == 100)
	{
		// 現在のデータを取得
		long long currentMeseta = mesetaCtrl.getCurrentMeseta();

		// 取得に成功
		if (currentMeseta >= 0)
		{
			// 記録中なら終了
			if (mesetaCtrl.currentMeseta.isRun)
			{
				if (!m_check_use_auto_finish.GetCheck())
					finishRecData(currentMeseta);
			}
			// 記録開始
			else
			{
				// 記録中は保険で自動終了の設定を変更不可にする
				bool auto_finish = m_check_use_auto_finish.GetCheck();
				int auto_finish_count = 0;

				// 自動終了設定時にカウントを読み取る
				if (auto_finish)
				{
					CString ini;
					m_auto_finish_count.GetWindowText(ini);
					auto_finish_count = _ttoi(ini);
					// 不正な値を入れられたら強制１秒
					if (auto_finish_count == 0)
					{
						auto_finish_count = 1;
						m_auto_finish_count.SetWindowText(L"1");
					}
				}
				m_check_use_auto_finish.EnableWindow(false);
				if (m_check_use_auto_finish.GetCheck())
					m_auto_finish_count.EnableWindow(false);

				// 計測開始
				mesetaCtrl.setCurrentData(currentMeseta, auto_finish_count);				

				// ステータスウィンドウリフレッシュ
				m_statusWindow->SetColor(iniData.dlgColor, iniData.fntColorR);

				// タイマーあり開始
				if (m_autoFinishTimerID == 0 && m_check_use_auto_finish.GetCheck())
				{
					m_autoFinishTimerID = 80;
					SetTimer(m_autoFinishTimerID, 1000, NULL);

					CString info;
					info.Format(L"%s\n(%03d)", L"記録中", auto_finish_count);
					m_statusWindow->SetInfo(info);
					
				}
				// タイマーなし開始
				else
				{
					CString info;
					info.Format(L"%s\n%s", L"記録終了", iniData.hotkeyRec.GetString());
					m_statusWindow->SetInfo(info);
				}
			}
		}
		// データ取得に失敗
		// ログディレクトリの指定が不正な状態でスタートを押した？
		else
		{
			// 保険
			MessageBox(L"NGSログファイルが読み取れません。\n設定から正しいフォルダを指定してください。", L"エラー");
		}
	}
	// 削除キー
	else if (nHotKeyId == 101 )
	{
		// 記録中ならデータを破棄して終了
		if (mesetaCtrl.currentMeseta.isRun)
		{
			mesetaCtrl.getCurrentMeseta();
			finishRecData(-1);
		}
		// 待機中ならログを上から順に削除
		else if (m_listCtrl.GetItemCount() > 0)
		{
			// 最新データの取り消し
			mesetaCtrl.pop();
			m_listCtrl.DeleteItem(0);
			m_statusWindow->SetLog(mesetaCtrl.mesetaData, iniData.disp_interval);
		}
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

// ダイアログの終了処理
void CMesetaDlg::OnDestroy()
{
	// ホットキーの削除
	DeleteHotkey();

	// タイマーが動いてたら全部殺す
	if (m_timerID != 0)
		KillTimer(m_timerID);

	if (m_autoFinishTimerID != 0)
		KillTimer(m_autoFinishTimerID);

	if (m_padTimerID != 0)
		KillTimer(m_padTimerID);

	// INIファイルに設定をすべて保存
	writeINI();
	
	CDialogEx::OnDestroy();
}

// WM_CLOSEをインターラプトして終了確認
void CMesetaDlg::OnClose()
{
#ifndef _DEBUG
	if (IDOK == MessageBox(L"本当に終了しますか？", L"確認", MB_OKCANCEL))
#endif
	{
		CDialogEx::OnClose();
	}
}

// リストコントロールのカスタムドロー
void CMesetaDlg::OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString strText;
	int row = 0;
	int col = 0;

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (pNMHDR)
	{
		LPNMLVCUSTOMDRAW pLVCD = (LPNMLVCUSTOMDRAW)pNMHDR;

		// 描画ステージ?
		switch (pLVCD->nmcd.dwDrawStage) {
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		case CDDS_ITEMPREPAINT:
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			break;
		case (CDDS_SUBITEM | CDDS_ITEMPREPAINT):
			row = (int)pLVCD->nmcd.dwItemSpec;
			col = pLVCD->iSubItem;
			// 行数によって、リストの背景色を変える
			strText = m_listCtrl.GetItemText(row, 0);
			if (_ttoi(strText) % 2 == 0)
			{
				pLVCD->clrTextBk = RGB(250, 230, 240);
				pLVCD->clrText = RGB(0, 0, 0);
			}
			else
			{
				pLVCD->clrTextBk = RGB(230, 240, 250);
				pLVCD->clrText = RGB(0, 0, 0);
			}
			*pResult = CDRF_NEWFONT;
			break;
		default:
			*pResult = CDRF_DODEFAULT;
			break;
		}
	}
}

// タイマー処理
void CMesetaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// 実行中以外は処理しない
	// （実行中以外はタイマーは動いてないはずなので保険程度）
	if (mesetaCtrl.isRunning())
	{
		// 経過時間タイマー
		if (nIDEvent == m_timerID )
		{
			CTime now = CTime::GetCurrentTime();
			m_statusWindow->SetTimeCount(mesetaCtrl.getElapsedTime(now));

			// 自動更新設定ONなら自動更新
			if (iniData.auto_refresh)
			{
				if (mesetaCtrl.elapsedTime % iniData.refresh_second == 0)
				{
					mesetaCtrl.getCurrentMeseta();
					m_statusWindow->SetTotalMeseta(mesetaCtrl.increasedMeseta, mesetaCtrl.mesetaPerHour);
				}
			}			
		}		
		// 自動終了タイマー
		else if (nIDEvent == m_autoFinishTimerID)
		{
			// カウントダウン更新
			CurrentMesetaData& data = mesetaCtrl.currentMeseta;
			data.auto_count--;
			CString info;
			info.Format(L"%s\n(%03d)", L"記録中", data.auto_count);
			m_statusWindow->SetInfo(info);

			// カウントダウン終了
			if (data.auto_count == 0)
			{
				long long currentMeseta = mesetaCtrl.getCurrentMeseta();
				finishRecData(currentMeseta);
			}
		}
		// ゲームパッド情報読み取りタイマー
		else if (nIDEvent == m_padTimerID)
		{
			//if (iniData.padUse)
			{
				// パッド情報を読み取りホットキーの動作を代行する
				XINPUT_STATE state;
				if ( XInputGetState(iniData.padNum, &state) == ERROR_SUCCESS )
				{
					if (state.Gamepad.wButtons)
					{
						if (padInfo.getTrigger(0, state.Gamepad.wButtons))
						{
							OnHotKey(100, 0, 0);
						}
						else if (padInfo.getTrigger(1, state.Gamepad.wButtons))
						{
							OnHotKey(101, 0, 0);
						}
					}
					padInfo.padBit = state.Gamepad.wButtons;
				}				
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

// ステータスウィンドウ最前面
void CMesetaDlg::OnBnClickedCheckPreSt()
{
	// チェックボックス読み取りと反映
	bool check = m_check_pre_status.GetCheck();
	m_statusWindow->setTop(check);
}

// 全てのウィンドウを最前面
void CMesetaDlg::OnBnClickedCheckPreLog()
{
	// チェックボックス読み取りと反映
	RECT r;
	GetWindowRect(&r);

	bool check = m_check_pre_log.GetCheck();
	if (check)
		m_check_pre_status.SetCheck(true);

	if (check)
	{
		SetWindowPos(&CMesetaDlg::wndTopMost, r.top, r.left, r.right, r.bottom, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
		m_check_pre_status.EnableWindow(false);
	}
	else
	{
		SetWindowPos(&CMesetaDlg::wndNoTopMost, r.top, r.left, r.right, r.bottom, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
		m_statusWindow->setTop(true);
		m_check_pre_status.EnableWindow(true);
	}
}

// ログウィンドウも透明化
void CMesetaDlg::OnBnClickedCheckTraLog()
{	
	// チェックボックス読み取りと反映
	int alpha = m_check_trans_log.GetCheck() ? m_window_alpha.GetPos() : 255;
	SetLayeredWindowAttributes(0, alpha, LWA_ALPHA);
}

// 透明度設定
void CMesetaDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// スライダーバーからα値を設定する
	if ((CSliderCtrl*)pScrollBar == &m_window_alpha)
	{
		int alpha = m_window_alpha.GetPos();
		m_statusWindow->SetAlpha(alpha);
		if (m_check_trans_log.GetCheck())
			SetLayeredWindowAttributes(0, alpha, LWA_ALPHA);
	}		

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 自動終了設定
void CMesetaDlg::OnBnClickedCheckAuto()
{
	// チェックボックス読み取りと反映
	bool check = m_check_use_auto_finish.GetCheck();
	if (check)
	{
		m_auto_finish_count.EnableWindow(true);
	}
	else
	{
		// 自動設定オフならカウントの設定を無効化する
		m_auto_finish_count.EnableWindow(false); 
	}
}

// スタートボタン
void CMesetaDlg::OnBnClickedButtonStart()
{
	// ログが残ってる場合は確認する
	if (m_listCtrl.GetItemCount() > 0)
	{
		if (IDCANCEL == MessageBox(L"データは全てはリセットされます。", L"確認", MB_OKCANCEL))
		{
			return;
		}
	}

	// 起動処理
	Init(true);	

	// 起動中は終了ボタン以外無効化する
	m_buttonStart.EnableWindow(false);
	m_buttonStart.SetWindowText(L"起動中");
	m_buttonEnd.EnableWindow(true);
	m_buttonConfig.EnableWindow(false);
}

// 終了ボタン
void CMesetaDlg::OnBnClickedButtonEnd()
{
	// 起動中以外は押されないはずだが
	if (mesetaCtrl.isRunning())
	{
		// 最終結果
		mesetaCtrl.getCurrentMeseta();
				
		// 記録中に終了ボタンが押されたらデータ破棄
		if (mesetaCtrl.currentMeseta.isRun)
		{			
			finishRecData(-1);
		}		

		// 記録をCSVファイルに書き出す
		mesetaCtrl.writeData();

		// タイマーが起動中なら殺す
		if (m_timerID != 0)
		{
			KillTimer(m_timerID);
			m_timerID = 0;
		}
		if (m_padTimerID != 0)
		{
			KillTimer(m_padTimerID);
			m_padTimerID = 0;
		}
		
		// スタートボタンと設定ボタンを有効化する
		m_buttonStart.EnableWindow(true);
		m_buttonStart.SetWindowText(L"スタート");
		m_buttonEnd.EnableWindow(false); // 終了ボタンは無効
		m_buttonConfig.EnableWindow(true);

		// ステータスウィンドウ更新
		m_statusWindow->SetInfo(L"保存完了\nスタートでリセット");
	}
}

// 設定ボタン
void CMesetaDlg::OnBnClickedButtonConfig()
{
	// ログが残ってる場合は確認する
	if (m_listCtrl.GetItemCount() > 0)
	{
		if (IDCANCEL == MessageBox(L"データは全てはリセットされます。", L"確認", MB_OKCANCEL))
		{
			return;
		}
	}

	// 初期化
	Init(false);

	// プロパティシート作成
	CPropertySheet PropSheet(_T("設定"));
	PropSheet.m_psh.dwFlags &= ~(PSH_HASHELP);

	// プロパティページ作成
	CPropHotkey propHotkey(this);
	CPropDirectory propDirectory(this);
	CPropFunction propFunctiuon(this);

	// シートにページを追加
	PropSheet.AddPage(&propFunctiuon);
	PropSheet.AddPage(&propHotkey);
	PropSheet.AddPage(&propDirectory);
	
	// 設定ダイアログをモーダルで実行
	if (PropSheet.DoModal() == IDOK)
	{
	}
}

// INI読み取りユーティリティ
// 文字列のカラーコードをCOLORREFに変換
COLORREF CMesetaDlg::Str2Col(CString col)
{
	COLORREF c = 0;
	if (col.GetLength() == 6)
	{
		CString r = col.Left(2);
		col.Delete(0, 2);
		CString g = col.Left(2);
		col.Delete(0, 2);
		CString b = col.Left(2);
		col.Delete(0, 2);

		c = RGB(wcstol(r, NULL, 16), wcstol(g, NULL, 16), wcstol(b, NULL, 16));
	}
	return c;
}

// INI読み取りユーティリティ
// COLORREFを文字列のカラーコードに変換
CString CMesetaDlg::Col2Str(COLORREF col)
{
	WORD r = col & 0xFF;
	WORD g = (col & 0xFF00) >> 8;
	WORD b = (col & 0xFF0000) >> 16;

	CString c;
	c.Format(L"%02X%02X%02X", r, g, b);

	return c;
}

// INI読み取りユーティリティ
// ホットキーの文字列をフラグに変換する
bool CMesetaDlg::Str2VK(CString key, UINT& mod, UINT& vk)
{
	key.Replace(L" ", L"");

	CString VK;
	std::vector<CString> MOD;
	for (;;)
	{
		int pos = key.Find(L"+");
		if (pos < 0)
		{
			VK = key.Left(1);
			break;
		}
		MOD.push_back(key.Left(pos));
		key.Delete(0, pos + 1);
	}

	if (MOD.size() == 0 || VK.GetLength() == 0)
		return false;

	mod = 0;
	for (size_t i = 0; i < MOD.size(); i++)
	{
		MOD[i].MakeUpper();
		if (MOD[i].CompareNoCase(L"ALT") == 0)
		{
			mod |= MOD_ALT;
		}
		else if (MOD[i].CompareNoCase(L"CTRL") == 0)
		{
			mod |= MOD_CONTROL;
		}
		else if (MOD[i].CompareNoCase(L"SHIFT") == 0)
		{
			mod |= MOD_SHIFT;
		}
		else if (MOD[i].CompareNoCase(L"WIN") == 0)
		{
			mod |= MOD_WIN;
		}
	}

	vk = (UINT)VK.GetBuffer()[0];

	return true;
}