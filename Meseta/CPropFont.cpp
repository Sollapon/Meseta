// CPropFont.cpp : 実装ファイル
// 
// 設定：表示タブ
//

#include "pch.h"
#include "Meseta.h"
#include "afxdialogex.h"
#include "CPropFont.h"


// CPropFont ダイアログ

IMPLEMENT_DYNAMIC(CPropFont, CMFCPropertyPage)

CPropFont::CPropFont(CMesetaDlg* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_PROPPAGE_FONT)
	, parentDlg(pParent)
	, logFont(LOGFONT())
	, m_bgColor(0)	
	, m_fontColorN(0)
	, m_fontColorR(0)
	, openWindow(false)
{

}

CPropFont::~CPropFont()
{
}

void CPropFont::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropFont, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_FONT, &CPropFont::OnBnClickedButtonChangeFont)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_FONT_COL1, &CPropFont::OnBnClickedButtonFontCol1)
	ON_BN_CLICKED(IDC_BUTTON_FONT_COL2, &CPropFont::OnBnClickedButtonFontCol2)
	ON_BN_CLICKED(IDC_BUTTON_FONT_COL3, &CPropFont::OnBnClickedButtonFontCol3)
END_MESSAGE_MAP()


// CPropFont メッセージ ハンドラー

// 初期化
BOOL CPropFont::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// ページを初期化
	CWnd* hWndSample1 = GetDlgItem(IDC_FONT_SAMPLE_1);
	CWnd* hWndSample2 = GetDlgItem(IDC_FONT_SAMPLE_2);

	// サンプル用フォント
	parentDlg->m_statusFont.GetLogFont(&logFont);
	sampleFont.CreateFontIndirect(&logFont);
	hWndSample1->SetFont(&sampleFont);
	hWndSample2->SetFont(&sampleFont);

	// 色変更用ブラシ	
	m_bgColor = parentDlg->iniData.dlgColor;
	m_fontColorN = parentDlg->iniData.fntColorN;
	m_fontColorR = parentDlg->iniData.fntColorR;
	m_brDlg.CreateSolidBrush(m_bgColor);

	// 中央寄せ
	long style = GetWindowLong(hWndSample1->GetSafeHwnd(), GWL_STYLE);
	style |= SS_CENTERIMAGE;
	SetWindowLong(hWndSample1->GetSafeHwnd(), GWL_STYLE, style);

	style = GetWindowLong(hWndSample2->GetSafeHwnd(), GWL_STYLE);
	style |= SS_CENTERIMAGE;
	SetWindowLong(hWndSample2->GetSafeHwnd(), GWL_STYLE, style);

	// フォント名
	GetDlgItem(IDC_EDIT_FONT)->SetWindowText(parentDlg->iniData.fontInfo.fontName);
	CString fntSize;
	fntSize.Format(L"%d", parentDlg->iniData.fontInfo.fontSize);
	GetDlgItem(IDC_EDIT_FONT2)->SetWindowText(fntSize);
	fontInfo = parentDlg->iniData.fontInfo;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// 適用またはOK時にINIデータを更新する
void CPropFont::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (openWindow)
	{
		parentDlg->iniData.fontInfo = fontInfo;
		parentDlg->iniData.dlgColor = m_bgColor;
		parentDlg->iniData.fntColorN = m_fontColorN;
		parentDlg->iniData.fntColorR = m_fontColorR;
		parentDlg->openStatusWindow();
		openWindow = false;
	}

	CMFCPropertyPage::OnOK();
}

// 終了処理
void CPropFont::OnDestroy()
{
	CMFCPropertyPage::OnDestroy();

	// 後始末
	sampleFont.DeleteObject();
	m_brDlg.DeleteObject();	
}

// サンプルの色変更
HBRUSH CPropFont::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CMFCPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	CRect rect;
	CBrush* pOldBrush;

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_PICTURE_BG:
			pOldBrush = (CBrush*)pDC->SelectObject(&m_brDlg);
			pDC->GetClipBox(&rect);
			pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);     // 対象領域を消去。
			pDC->SelectObject(pOldBrush);
			break;
		case IDC_FONT_SAMPLE_1:
			pDC->SetTextColor(m_fontColorN);
			return (HBRUSH)m_brDlg;
			break;
		case IDC_FONT_SAMPLE_2:
			pDC->SetTextColor(m_fontColorR);
			return (HBRUSH)m_brDlg;
			break;
		default:
			break;
		}
	}

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

// フォント変更ボタン
void CPropFont::OnBnClickedButtonChangeFont()
{
	// フォント設定ダイアログの初期化
	CFontDialog fontDlg;
	fontDlg.m_cf.lpLogFont = &logFont;
	fontDlg.m_cf.Flags ^= CF_EFFECTS;
	fontDlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;

	if (fontDlg.DoModal() == IDOK)
	{
		// 指定情報を一時バッファに記憶
		fontInfo.fontName = fontDlg.GetFaceName();
		fontInfo.fontSize = fontDlg.GetSize()/10;
		fontInfo.fontWeight = fontDlg.GetWeight();
		fontInfo.fontItalic = fontDlg.IsItalic();
		fontDlg.GetCurrentFont(&logFont);
				
		// サンプル用フォント作成
		sampleFont.DeleteObject();
		sampleFont.CreateFontIndirect(&logFont);
		GetDlgItem(IDC_FONT_SAMPLE_1)->SetFont(&sampleFont);
		GetDlgItem(IDC_FONT_SAMPLE_2)->SetFont(&sampleFont);

		// フォント名表示
		GetDlgItem(IDC_EDIT_FONT)->SetWindowText(fontInfo.fontName);
		CString fntSize;
		fntSize.Format(L"%d", fontInfo.fontSize);
		GetDlgItem(IDC_EDIT_FONT2)->SetWindowText(fntSize);

		SetModified();
		openWindow = true;
	}
}

// 背景色の変更
void CPropFont::OnBnClickedButtonFontCol1()
{
	CColorDialog colDlg;
	colDlg.m_cc.Flags = colDlg.m_cc.Flags | CC_RGBINIT | CC_FULLOPEN;
	colDlg.m_cc.rgbResult = m_bgColor;

	if (colDlg.DoModal() == IDOK)
	{
		COLORREF getCol = colDlg.GetColor();
		if (getCol != m_bgColor)
		{
			m_bgColor = getCol;
			m_brDlg.DeleteObject();
			m_brDlg.CreateSolidBrush(m_bgColor);
			InvalidateSample();
			SetModified();
			openWindow = true;
		}
	}
}

// 通常フォントカラー
void CPropFont::OnBnClickedButtonFontCol2()
{
	CColorDialog colDlg;
	colDlg.m_cc.Flags = colDlg.m_cc.Flags | CC_RGBINIT | CC_FULLOPEN;
	colDlg.m_cc.rgbResult = m_fontColorN;

	if (colDlg.DoModal() == IDOK)
	{
		COLORREF getCol = colDlg.GetColor();
		if (getCol != m_fontColorN)
		{
			m_fontColorN = getCol;
			InvalidateSample();
			SetModified();
			openWindow = true;
		}
	}
}

// 記録中フォントカラー
void CPropFont::OnBnClickedButtonFontCol3()
{
	CColorDialog colDlg;
	colDlg.m_cc.Flags = colDlg.m_cc.Flags | CC_RGBINIT | CC_FULLOPEN;
	colDlg.m_cc.rgbResult = m_fontColorR;

	if (colDlg.DoModal() == IDOK)
	{
		COLORREF getCol = colDlg.GetColor();
		if (getCol != m_fontColorR)
		{
			m_fontColorR = getCol;
			InvalidateSample();
			SetModified();
			openWindow = true;
		}
	}
}

// サンプル更新
void CPropFont::InvalidateSample()
{
	GetDlgItem(IDC_PICTURE_BG)->Invalidate();
	GetDlgItem(IDC_FONT_SAMPLE_1)->Invalidate();
	GetDlgItem(IDC_FONT_SAMPLE_2)->Invalidate();
}