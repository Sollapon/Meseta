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


void CStatusWindow::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	delete this;
}


LRESULT CStatusWindow::OnNcHitTest(CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if ((HTCLIENT == lResult))
	{
		return HTCAPTION;
	}
	return lResult;
}


BOOL CStatusWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	// ウィンドウの拡張スタイルを変更
	ModifyStyleEx(0, WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 255, LWA_ALPHA);

	m_brDlg.CreateSolidBrush(RGB(0, 0, 0));
	m_fontColor = RGB(255, 255, 255);

	GetDlgItem(IDC_STATIC_MESETA1)->SetWindowText(L"[01]\n17,000,000\n(1,500,000/s)");
	GetDlgItem(IDC_STATIC_MESETA2)->SetWindowText(L"[01]\n17,000,000\n(150000/s)");
	GetDlgItem(IDC_STATIC_MESETA3)->SetWindowText(L"[01]\n17,000,000\n(150000/s)");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

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

void CStatusWindow::SetColor(COLORREF dlgColor, COLORREF fontColor)
{
	SetFontColor(fontColor);
	SetColor(dlgColor);
}


// フォントカラー設定
void CStatusWindow::SetFontColor(COLORREF c)
{
	m_fontColor = c;
}

void CStatusWindow::SetTimeCount(CString time)
{
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(time);
}


void CStatusWindow::SetTotalMeseta(CString meseta)
{	
	GetDlgItem(IDC_STATIC_MPH)->SetWindowText(meseta);
}
void CStatusWindow::SetTotalMeseta(long long meseta, long long mph)
{
	CString strMeseta = MesetaDataCtrl::MakeBigNumber(meseta);
	CString strMPH = MesetaDataCtrl::MakeBigNumber(mph);
	CString strTotal;
	strTotal.Format(L"%s\n(%s/h)", strMeseta.GetString(), strMPH.GetString());

	SetTotalMeseta(strTotal);
}


void CStatusWindow::clearLog()
{
	GetDlgItem(IDC_STATIC_MESETA1)->SetWindowText(L"");
	GetDlgItem(IDC_STATIC_MESETA2)->SetWindowText(L"");
	GetDlgItem(IDC_STATIC_MESETA3)->SetWindowText(L"");
}

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
		if (idx >= 3)
			break;
		else
			GetDlgItem(id[idx++])->SetWindowText(text);
	}
}

void CStatusWindow::SetInfo(CString info)
{
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(info);
}


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


HBRUSH CStatusWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
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


	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


void CStatusWindow::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_brDlg.DeleteObject();

	RECT r;
	CWinApp* pApp = AfxGetApp();
	GetWindowRect(&r);
	pApp->WriteProfileInt(L"DLG_POS", L"SUB_TOP", r.top);
	pApp->WriteProfileInt(L"DLG_POS", L"SUB_LEFT", r.left);
}


BOOL CStatusWindow::PreTranslateMessage(MSG* pMsg)
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
	else if (WM_SYSCOMMAND)
	{
		if (pMsg->wParam == SC_CLOSE)
		{
			return(TRUE);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
