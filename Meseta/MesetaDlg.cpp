
// MesetaDlg.cpp : 実装ファイル
//

//#include "pch.h"
#include "framework.h"
#include "Meseta.h"
#include "MesetaDlg.h"
#include "afxdialogex.h"

#include "CPropHotkey.h"
#include "CPropDirectory.h"
#include "CPropFunction.h"


#pragma comment(lib, "xinput9_1_0.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "EnumFile.h"
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

	m_listCtrl.InsertColumn(0, _T("id"), LVCFMT_CENTER, 25);
	m_listCtrl.InsertColumn(1, _T("メセタ"), LVCFMT_CENTER, 90);
	m_listCtrl.InsertColumn(2, _T("開始時刻"), LVCFMT_CENTER, 90);
	m_listCtrl.InsertColumn(3, _T("記録時間"), LVCFMT_CENTER, 70);
	m_listCtrl.InsertColumn(4, _T("メセタ/s"), LVCFMT_CENTER, 80);
	m_listCtrl.InsertColumn(5, _T("インターバル"), LVCFMT_CENTER, 70);

	m_buttonEnd.EnableWindow(false);

	// ウィンドウの拡張スタイルを変更
	ModifyStyleEx(0, WS_EX_LAYERED);

	// レイヤードウィンドウの不透明度と透明のカラーキー
	SetLayeredWindowAttributes(0, 255, LWA_ALPHA);
	
	// サブウィンドウ作成
	m_statusWindow = new CStatusWindow(this);
	m_statusWindow->Create(IDD_FORMVIEW, this);
	
	clearStatusWindow();

	checkIniFile();

	// INIファイルの読み込み
	readINI();

	// サブカラー設定	
	m_statusWindow->SetColor(iniData.dlgColor, iniData.fntColorN);

	// ホットキー設定
	SetHotkey();
	
	// 初期化
	Init(false);	
	
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


bool CMesetaDlg::SetHotkey()
{
	UINT mod, vk;
	CMesetaDlg::Str2VK(iniData.hotkeyRec, mod, vk);
	BOOL ret = ::RegisterHotKey(GetSafeHwnd(), 100, mod, vk);
	CString err = L"ホットキーが既に別のアプリで使用されています。\n設定から使用可能なキーを割り当ててください。\n";
	if (!ret)
	{
		CString key = iniData.hotkeyRec;
		MessageBox(err+L"("+key + L")", L"エラー");
	}

	if (iniData.hotkeyUseDell)
	{
		CMesetaDlg::Str2VK(iniData.hotkeyDel, mod, vk);
		ret = ::RegisterHotKey(GetSafeHwnd(), 101, mod, vk);
		if (!ret)
		{
			CString key = iniData.hotkeyDel;
			MessageBox(err + L"(" + key + L")", L"エラー");
		}
	}

	return ret;
}

bool CMesetaDlg::DeleteHotkey()
{
	BOOL ret = ::UnregisterHotKey(GetSafeHwnd(), 100);
	ret = ::UnregisterHotKey(GetSafeHwnd(), 101);

	return ret;
}

void CMesetaDlg::clearStatusWindow()
{
	if (m_statusWindow)
	{
		m_statusWindow->SetTimeCount(0, 0, 0);
		m_statusWindow->SetTotalMeseta(0, 0);
		m_statusWindow->clearLog();
	}
}


bool CMesetaDlg::checkIniFile()
{
	CFileFind find;
	CString filePath = _T("Meseta.ini");
	if (find.FindFile(filePath))
	{ 
		
	}
	// 無かったら作る
	else
	{
		FILE* fp;
		_wfopen_s(& fp, filePath, _T("w, ccs=UTF-16LE"));
		CStdioFile file(fp);
		if (!file)
			return false;			

		CString path = L"ここに正しいngs_logフォルダのパスを設定してください。";

		HKEY hKye;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), NULL, KEY_QUERY_VALUE, &hKye) == ERROR_SUCCESS)
		{			
			DWORD dwType;
			TCHAR lpData[1024];
			DWORD dwDataSize;
			dwDataSize = sizeof(lpData) / sizeof(lpData[0]);
			LSTATUS err = RegQueryValueEx(hKye, TEXT("Personal"), 0, &dwType, (LPBYTE)lpData , &dwDataSize);
			RegCloseKey(hKye);

			path = CString(L"MGS_LOG=") + CString(lpData) + CString(DEFAULT_NGS_LOG) + CString(L"\n");
		}

		file.WriteString(L"[LOG_DIR]\n");
		file.WriteString(path);

		file.Close();
	}
	return true;
}


BOOL CMesetaDlg::readINI()
{
	// NGS_LOGフォルダへのパスを読み込み
	CWinApp* pApp = AfxGetApp();
	iniData.ngs_log_path = pApp->GetProfileString(L"LOG_DIR", L"MGS_LOG", CString(DEFAULT_DOC)+CString(DEFAULT_NGS_LOG));

	BOOL ret = mesetaCtrl.Init(iniData.ngs_log_path);

	if (!ret)
	{
		MessageBox(L"NGSログファイルが読み取れません。\n設定から正しいフォルダを指定してください。", L"エラー");
	}
	
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

	padInfo.clear();
	padInfo.setBit(0, iniData.padRec);
	padInfo.setBit(1, iniData.padDel);

	return ret;
}


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

		c = RGB( wcstol(r, NULL, 16), wcstol(g, NULL, 16), wcstol(b, NULL, 16));
	}

	return c;
}

CString CMesetaDlg::Col2Str(COLORREF col)
{
	WORD r = col & 0xFF;
	WORD g = (col & 0xFF00) >> 8;
	WORD b = (col & 0xFF0000) >> 16;

	CString c;
	c.Format(L"%02X%02X%02X", r, g, b);

	return c;
}


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

	if (MOD.size() == 0 || VK.GetLength()==0)
		return false;

	mod = 0;
	for (size_t i = 0; i < MOD.size(); i++)
	{
		MOD[i].MakeUpper();
		if (MOD[i].CompareNoCase(L"ALT")==0)
		{
			mod |= MOD_ALT;
		}
		else if (MOD[i].CompareNoCase(L"CTRL")==0)
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



BOOL CMesetaDlg::writeINI()
{
	RECT r;
	GetWindowRect(&r);
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(L"DLG_POS", L"TOP", r.top);
	pApp->WriteProfileInt(L"DLG_POS", L"LEFT", r.left);

	pApp->WriteProfileString(L"LOG_DIR", L"MGS_LOG", iniData.ngs_log_path);
	pApp->WriteProfileString(L"DLG_POS", L"SAVE", iniData.pos_save?L"TRUE":L"FALSE");
	pApp->WriteProfileString(L"DISP", L"INTERVAL", iniData.disp_interval ? L"TRUE" : L"FALSE");


	pApp->WriteProfileString(L"TOP_WINDOW", L"LOG", (bool)m_check_pre_log.GetCheck()?L"TRUE":L"FALSE");
	pApp->WriteProfileString(L"TOP_WINDOW", L"STATUS", (bool)m_check_pre_status.GetCheck() ? L"TRUE" : L"FALSE");


	pApp->WriteProfileInt(L"TRANSPARENT", L"ALPHA", m_window_alpha.GetPos());
	pApp->WriteProfileString(L"TRANSPARENT", L"TRANS_LOG", (bool)m_check_trans_log.GetCheck() ? L"TRUE" : L"FALSE");


	pApp->WriteProfileString(L"AUTO_FINISH", L"USE", (bool)m_check_use_auto_finish.GetCheck() ? L"TRUE" : L"FALSE");
	CString ini;
	m_auto_finish_count.GetWindowText(ini);
	pApp->WriteProfileInt(L"AUTO_FINISH", L"SECOND", _ttoi(ini));

	pApp->WriteProfileString(L"AUTO_REFRESH", L"USE", iniData.auto_refresh ? L"TRUE" : L"FALSE");
	pApp->WriteProfileInt(L"AUTO_REFRESH", L"SECOND", iniData.refresh_second);


	pApp->WriteProfileString(L"COLORE", L"DIALOG", CMesetaDlg::Col2Str(iniData.dlgColor));
	pApp->WriteProfileString(L"COLORE", L"FONT_N", CMesetaDlg::Col2Str(iniData.fntColorN));
	pApp->WriteProfileString(L"COLORE", L"FONT_R", CMesetaDlg::Col2Str(iniData.fntColorR));


	pApp->WriteProfileString(L"HOTKEY", L"USE_DELL", (bool)iniData.hotkeyUseDell ? L"TRUE" : L"FALSE");
	pApp->WriteProfileString(L"HOTKEY", L"REC", iniData.hotkeyRec);
	pApp->WriteProfileString(L"HOTKEY", L"DEL", iniData.hotkeyDel);


	pApp->WriteProfileString(L"GAMEPAD", L"USE", (bool)iniData.padUse ? L"TRUE" : L"FALSE");
	pApp->WriteProfileString(L"GAMEPAD", L"REC", iniData.padRec);
	pApp->WriteProfileString(L"GAMEPAD", L"DEL", iniData.padDel);
	pApp->WriteProfileInt(L"GAMEPAD", L"NUM", iniData.padNum);

	return true;
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


BOOL CMesetaDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
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

void CMesetaDlg::finishRecData(long long currentMeseta)
{
	// 記録終了
	mesetaCtrl.endCurrentData(currentMeseta);

	if (currentMeseta >= 0)
	{
		// 計測終了
		MesetaData md = mesetaCtrl.currentMeseta;
		md.idx = (int)mesetaCtrl.mesetaData.size() + 1;

		long long lastTime = (mesetaCtrl.mesetaData.size() > 0) ? mesetaCtrl.mesetaData.back().end.GetTime() : mesetaCtrl.initialTime.GetTime();
		md.interval = md.start.GetTime() - lastTime;		

		mesetaCtrl.mesetaData.push_back(md);

		// コントロール描画
		CString addText;

		addText.Format(L"%d", md.idx);
		m_listCtrl.InsertItem(0, addText);

		//addText.Format(L"%lld", md.meseta);
		addText = MesetaDataCtrl::MakeBigNumber(md.meseta);
		m_listCtrl.SetItemText(0, 1, addText);

		addText = md.start.Format(L"%m/%d %H:%M:%S");
		m_listCtrl.SetItemText(0, 2, addText);

		//long long time = md.end.GetTime() - md.start.GetTime();
		addText.Format(L"%lld秒", md.recTime);
		m_listCtrl.SetItemText(0, 3, addText);

		//addText.Format(L"%lld", md.mps);
		addText = MesetaDataCtrl::MakeBigNumber(md.mps);
		addText += L"/s";
		m_listCtrl.SetItemText(0, 4, addText);
		
		
		m_listCtrl.SetItemText(0, 5, MesetaDataCtrl::Time2Min(md.interval));
	}	

	m_check_use_auto_finish.EnableWindow(true);
	if (m_check_use_auto_finish.GetCheck())
		m_auto_finish_count.EnableWindow(true);

	// ステータスウィンドウ更新
	CTime now = CTime::GetCurrentTime();
	m_statusWindow->SetTimeCount(mesetaCtrl.getElapsedTime(now));
	m_statusWindow->SetColor(iniData.dlgColor, iniData.fntColorN);
	m_statusWindow->SetTotalMeseta(mesetaCtrl.increasedMeseta, mesetaCtrl.mesetaPerHour);
	m_statusWindow->SetLog(mesetaCtrl.mesetaData, iniData.disp_interval);

	if (m_autoFinishTimerID != 0)
	{
		KillTimer(m_autoFinishTimerID);
		m_autoFinishTimerID = 0;
	}

	CString info;
	info.Format(L"%s\n%s", L"記録開始", iniData.hotkeyRec.GetString());
	m_statusWindow->SetInfo(info);
}


void CMesetaDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if (!mesetaCtrl.isRunning())
	{
	}
	else if (nHotKeyId == 100)
	{
		long long currentMeseta = mesetaCtrl.getCurrentMeseta();

		if (currentMeseta >= 0)
		{
			if (mesetaCtrl.currentMeseta.isRun)
			{
				if (!m_check_use_auto_finish.GetCheck())
					finishRecData(currentMeseta);
			}
			else
			{
				bool auto_finish = m_check_use_auto_finish.GetCheck();
				int auto_finish_count = 0;

				if (auto_finish)
				{
					CString ini;
					m_auto_finish_count.GetWindowText(ini);
					auto_finish_count = _ttoi(ini);
					if (auto_finish_count == 0)
					{
						auto_finish_count = 1;
						m_auto_finish_count.SetWindowText(L"1");
					}
				}

				// 計測開始
				mesetaCtrl.setCurrentData(currentMeseta, auto_finish_count);

				m_check_use_auto_finish.EnableWindow(false);
				if (m_check_use_auto_finish.GetCheck())
					m_auto_finish_count.EnableWindow(false);

				m_statusWindow->SetColor(iniData.dlgColor, iniData.fntColorR);


				// タイマー始動
				if (m_autoFinishTimerID == 0 && m_check_use_auto_finish.GetCheck())
				{
					m_autoFinishTimerID = 80;
					SetTimer(m_autoFinishTimerID, 1000, NULL);

					CString info;
					info.Format(L"%s\n(%03d)", L"記録中", auto_finish_count);
					m_statusWindow->SetInfo(info);

					
				}
				else
				{
					CString info;
					info.Format(L"%s\n%s", L"記録終了", iniData.hotkeyRec.GetString());
					m_statusWindow->SetInfo(info);
				}
			}
		}
		else
		{
			MessageBox(L"NGSログファイルが読み取れません。\n設定から正しいフォルダを指定してください。", L"エラー");
		}
	}
	else if (nHotKeyId == 101 )
	{
		if (mesetaCtrl.currentMeseta.isRun)
		{
			mesetaCtrl.getCurrentMeseta();
			finishRecData(-1);
		}
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


void CMesetaDlg::OnDestroy()
{
	// ホットキーの削除
	DeleteHotkey();

	if (m_timerID != 0)
		KillTimer(m_timerID);

	if (m_autoFinishTimerID != 0)
		KillTimer(m_autoFinishTimerID);

	if (m_padTimerID != 0)
		KillTimer(m_padTimerID);


	// ウィンドウの位置を記録
	writeINI();

	
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}


void CMesetaDlg::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
#ifndef _DEBUG
	if (IDOK == MessageBox(L"本当に終了しますか？", L"確認", MB_OKCANCEL))
#endif
	{
		CDialogEx::OnClose();
	}
}


void CMesetaDlg::OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString strText;
	int row = 0;
	int col = 0;

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
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

			// 性別によって、リストの背景色を変える
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


void CMesetaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if (mesetaCtrl.isRunning())
	{
		if (nIDEvent == m_timerID )
		{
			CTime now = CTime::GetCurrentTime();
			m_statusWindow->SetTimeCount(mesetaCtrl.getElapsedTime(now));

			// 自動更新
			if (iniData.auto_refresh)
			{
				if (mesetaCtrl.elapsedTime % iniData.refresh_second == 0)
				{
					mesetaCtrl.getCurrentMeseta();
					m_statusWindow->SetTotalMeseta(mesetaCtrl.increasedMeseta, mesetaCtrl.mesetaPerHour);
				}
			}			
		}		
		else if (nIDEvent == m_autoFinishTimerID)
		{
			CurrentMesetaData& data = mesetaCtrl.currentMeseta;
			data.auto_count--;
			CString info;
			info.Format(L"%s\n(%03d)", L"記録中", data.auto_count);
			m_statusWindow->SetInfo(info);

			if (data.auto_count == 0)
			{
				long long currentMeseta = mesetaCtrl.getCurrentMeseta();
				finishRecData(currentMeseta);
			}
		}
		else if (nIDEvent == m_padTimerID)
		{
			//if (iniData.padUse)
			{

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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	bool check = m_check_pre_status.GetCheck();
	m_statusWindow->setTop(check);
}

// 全てのウィンドウを最前面
void CMesetaDlg::OnBnClickedCheckPreLog()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	
	int alpha = m_check_trans_log.GetCheck() ? m_window_alpha.GetPos() : 255;
	SetLayeredWindowAttributes(0, alpha, LWA_ALPHA);

}


void CMesetaDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if ((CSliderCtrl*)pScrollBar == &m_window_alpha)
	{
		int alpha = m_window_alpha.GetPos();
		m_statusWindow->SetAlpha(alpha);
		if (m_check_trans_log.GetCheck())
			SetLayeredWindowAttributes(0, alpha, LWA_ALPHA);
	}		

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMesetaDlg::OnBnClickedCheckAuto()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	bool check = m_check_use_auto_finish.GetCheck();
	if (check)
	{
		m_auto_finish_count.EnableWindow(true);
	}
	else
	{
		m_auto_finish_count.EnableWindow(false);
	}
}

void CMesetaDlg::Init(bool run)
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	bool ret = mesetaCtrl.Init(iniData.ngs_log_path, run);

	m_listCtrl.DeleteAllItems();

	// サブウィンドウ初期化
	clearStatusWindow();

	if (run)
	{
		// タイマー始動
		if (m_timerID == 0)
		{
			m_timerID = 1;
			SetTimer(m_timerID, 1000, NULL);
		}

		if (iniData.padUse && m_padTimerID == 0)
		{
			m_padTimerID = 105;
			SetTimer(m_padTimerID, 16, NULL);
		}

		CString info;
		info.Format(L"%s\n%s", L"記録開始", iniData.hotkeyRec.GetString());
		m_statusWindow->SetInfo(info);
	}
	else
	{
		m_statusWindow->SetInfo(L"スタートで起動");
	}
}


void CMesetaDlg::OnBnClickedButtonStart()
{
	if (m_listCtrl.GetItemCount() > 0)
	{
		if (IDCANCEL == MessageBox(L"データは全てはリセットされます。", L"確認", MB_OKCANCEL))
		{
			return;
		}
	}

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	Init(true);
	m_buttonStart.EnableWindow(false);
	m_buttonStart.SetWindowText(L"起動中");
	m_buttonEnd.EnableWindow(true);
	m_buttonConfig.EnableWindow(false);
}

void CMesetaDlg::OnBnClickedButtonEnd()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (mesetaCtrl.isRunning())
	{
		// 最終結果
		mesetaCtrl.getCurrentMeseta();
				
		if (mesetaCtrl.currentMeseta.isRun)
		{			
			finishRecData(-1);
		}		

		mesetaCtrl.writeData();

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
				
		m_buttonStart.EnableWindow(true);
		m_buttonStart.SetWindowText(L"スタート");
		m_buttonEnd.EnableWindow(false);
		m_buttonConfig.EnableWindow(true);


		m_statusWindow->SetInfo(L"保存完了\nスタートでリセット");
	}
}


void CMesetaDlg::OnBnClickedButtonConfig()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_listCtrl.GetItemCount() > 0)
	{
		if (IDCANCEL == MessageBox(L"データは全てはリセットされます。", L"確認", MB_OKCANCEL))
		{
			return;
		}
	}

	Init(false);

	CPropertySheet PropSheet(_T("設定"));
	PropSheet.m_psh.dwFlags &= ~(PSH_HASHELP);

	CPropHotkey propHotkey(this);
	CPropDirectory propDirectory(this);
	CPropFunction propFunctiuon(this);

	PropSheet.AddPage(&propFunctiuon);
	PropSheet.AddPage(&propHotkey);
	PropSheet.AddPage(&propDirectory);
	

	if (PropSheet.DoModal() == IDOK)
	{
	}
}
