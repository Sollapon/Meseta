
// Meseta.cpp : アプリケーションのクラス動作を定義します。
//

//#include "pch.h"
#include "framework.h"
#include "Meseta.h"
#include "MesetaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMesetaApp

BEGIN_MESSAGE_MAP(CMesetaApp, CWinApp)
	//ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()


// CMesetaApp の構築

CMesetaApp::CMesetaApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CMesetaApp オブジェクト

CMesetaApp theApp;


// CMesetaApp の初期化

BOOL CMesetaApp::InitInstance()
{

	// 多重起動を禁止する
	m_hMutex = CreateMutex(FALSE, 0, L"Meseta.exe");
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if (m_hMutex)
		{
			CloseHandle(m_hMutex);
		}		
		return FALSE;  // InitInstance関数はFALSEを返すと終了する
	}

	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
	CShellManager *pShellManager = new CShellManager;

	// MFC コントロールでテーマを有効にするために、"Windows ネイティブ" のビジュアル マネージャーをアクティブ化
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	
	// レジストリは使わない
	//SetRegistryKey(_T("Meseta"));
	
	// INIファイルを実行ファイルと同じ場所にする
	TCHAR path[_MAX_PATH + 1];
	GetModuleFileName(NULL, path, _MAX_PATH);
	PathRenameExtension(path, TEXT(".ini"));
	free((void*)m_pszProfileName);
	m_pszProfileName = _tcsdup(path);
		
	CMesetaDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: ダイアログの作成に失敗しました。アプリケーションは予期せずに終了します。\n");
		TRACE(traceAppMsg, 0, "警告: ダイアログで MFC コントロールを使用している場合、#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS を指定できません。\n");
	}

	// 上で作成されたシェル マネージャーを削除します。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

// 終了時にミューテックス削除
int CMesetaApp::ExitInstance()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	// Mutexの所有権を解放
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);

	return CWinApp::ExitInstance();
}

// ヘルプ操作は無効化
afx_msg void CMesetaApp::OnHelp()
{
	//ShellExecute(m_pMainWnd->GetSafeHwnd(), L"open", L"https://docs.google.com/spreadsheets/d/1qWgByQqiZloYVrno6Mhkq_w8FEY5__-EFlBvo-ua8SY/", NULL, NULL, SW_SHOW);
	return;
	//return CWinApp::OnHelp();
}

