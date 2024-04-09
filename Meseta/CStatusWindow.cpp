// CStatusWindow.cpp : 実装ファイル
//

#include "pch.h"
#include "Meseta.h"
#include "afxdialogex.h"
#include "CStatusWindow.h"


// CStatusWindow ダイアログ

IMPLEMENT_DYNAMIC(CStatusWindow, CDialogEx)

CStatusWindow::CStatusWindow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FORMVIEW, pParent)
{

}

CStatusWindow::~CStatusWindow()
{
}

void CStatusWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStatusWindow, CDialogEx)
	ON_WM_NCDESTROY()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CStatusWindow メッセージ ハンドラー

// 初期化
BOOL CStatusWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ウィンドウの拡張スタイルを変更
	ModifyStyleEx(0, WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 255, LWA_ALPHA);

	// 色変更用ブラシ
	m_brDlg.CreateSolidBrush(RGB(0, 0, 0));
	m_fontColor = RGB(255, 255, 255);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// メッセージ処理
BOOL CStatusWindow::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		// ダイアログのEnterとEscの規定処理を殺す
		if (VK_RETURN == pMsg->wParam) {
			return TRUE;
		}
		else if (VK_ESCAPE == pMsg->wParam) {
			return TRUE;
		}
	}
	else if (WM_SYSCOMMAND)
	{
		// 親ウィンドウからのクローズ命令以外を受け付けない
		if (pMsg->wParam == SC_CLOSE)
		{
			return(TRUE);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

// 終了処理
void CStatusWindow::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();

	// 自分自身を削除する
	delete this;
}

// ウィンドウ全体でドラッグできるようにする
LRESULT CStatusWindow::OnNcHitTest(CPoint point)
{
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if ((HTCLIENT == lResult))
	{
		return HTCAPTION;
	}
	return lResult;
}

// ダイアログの色変更対応
HBRUSH CStatusWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{

	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_fontColor);
		return (HBRUSH)m_brDlg;

	case CTLCOLOR_DLG:
		pDC->SetTextColor(m_fontColor);
		return (HBRUSH)m_brDlg;
	default:
		break;
	}

	return hbr;
}

// 終了処理
void CStatusWindow::OnDestroy()
{
	CDialogEx::OnDestroy();

	// ブラシ削除
	m_brDlg.DeleteObject();

	// 終了時の座標をINIに保存
	RECT r;
	CWinApp* pApp = AfxGetApp();
	GetWindowRect(&r);
	pApp->WriteProfileInt(L"DLG_POS", L"SUB_TOP", r.top);
	pApp->WriteProfileInt(L"DLG_POS", L"SUB_LEFT", r.left);
}

// 以下情報更新関数
//
// α値設定
void CStatusWindow::SetAlpha(BYTE alpha)
{
	SetLayeredWindowAttributes(0, alpha, LWA_ALPHA);
}

// 背景カラー設定
void CStatusWindow::SetColor(COLORREF c)
{
	if (m_brDlg.DeleteObject())
		m_brDlg.CreateSolidBrush(c);

	SetBackgroundColor(c);
}

// フォントカラー設定
void CStatusWindow::SetFontColor(COLORREF c)
{
	m_fontColor = c;
}

// 背景とフォント同時変更
void CStatusWindow::SetColor(COLORREF dlgColor, COLORREF fontColor)
{
	SetFontColor(fontColor);
	SetColor(dlgColor);
}

// 経過時間更新
void CStatusWindow::SetTimeCount(CString time)
{
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(time);
}

// 総メセタ更新
void CStatusWindow::SetTotalMeseta(CString meseta)
{	
	GetDlgItem(IDC_STATIC_MPH)->SetWindowText(meseta);
}

// 総メセタ更新
void CStatusWindow::SetTotalMeseta(long long meseta, long long mph)
{
	CString strMeseta = MesetaDataCtrl::MakeBigNumber(meseta);
	CString strMPH = MesetaDataCtrl::MakeBigNumber(mph);
	CString strTotal;
	strTotal.Format(L"%s\n(%s/h)", strMeseta.GetString(), strMPH.GetString());

	SetTotalMeseta(strTotal);
}

// ログを消去する
void CStatusWindow::clearLog()
{
	GetDlgItem(IDC_STATIC_MESETA1)->SetWindowText(L"");
	GetDlgItem(IDC_STATIC_MESETA2)->SetWindowText(L"");
	GetDlgItem(IDC_STATIC_MESETA3)->SetWindowText(L"");
}

// ログを更新
void CStatusWindow::SetLog(const std::vector <MesetaData>& mesetaData, bool change)
{

	DWORD id[3] =
	{
		IDC_STATIC_MESETA1,
		IDC_STATIC_MESETA2,
		IDC_STATIC_MESETA3
	};

	clearLog();

	if (mesetaData.size() == 0) return;

	int start = (int)mesetaData.size() - 1;

	int idx = 0;
	for (int i = start; i >= 0 ; i--)
	{
		MesetaData data = mesetaData[i];
		CString m = MesetaDataCtrl::MakeBigNumber(data.meseta);
		CString mps = change ? MesetaDataCtrl::Time2Min(data.interval) : (MesetaDataCtrl::MakeBigNumber(data.mps)+ L"/s") ;
		CString text;
		text.Format(L"[%d]\n%s\n(%s)", data.idx, m.GetString(), mps.GetString());
		
		// 最新３件のみ表示
		if (idx < 3)
		{
			GetDlgItem(id[idx])->SetWindowText(text.GetString());
			idx++;
		}
		else
			break;
	}
}

// 情報文字列更新
void CStatusWindow::SetInfo(CString info)
{
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(info);
}

// 最前面設定
void CStatusWindow::setTop(bool b, int top, int left)
{	
	UINT flag = (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	if (top < 0 || left < 0)
	{
		top = left = 0;
	}
	else
	{
		flag = (SWP_NOSIZE | SWP_SHOWWINDOW);
	}

	SetWindowPos( b ? &CStatusWindow::wndTopMost : &CStatusWindow::wndNoTopMost, left, top, 0, 0, flag);	
}

// フォント設定
void CStatusWindow::setCtrlFont(CFont* font)
{
	SetFont(font, false);
	SendMessageToDescendants(WM_SETFONT,(WPARAM)font->m_hObject,MAKELONG(FALSE, 0),	FALSE);
}
