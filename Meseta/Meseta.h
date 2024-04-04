
// Meseta.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMesetaApp:
// このクラスの実装については、Meseta.cpp を参照してください
//

class CMesetaApp : public CWinApp
{
public:
	CMesetaApp();

// オーバーライド
public:
	virtual BOOL InitInstance();
	afx_msg void OnHelp();

// 実装

	DECLARE_MESSAGE_MAP()

private:
	HANDLE m_hMutex;
public:
	virtual int ExitInstance();
};

extern CMesetaApp theApp;
