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
END_MESSAGE_MAP()


// CPropFont メッセージ ハンドラー

// 初期化
BOOL CPropFont::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// ページを初期化
	parentDlg->m_statusFont.GetLogFont(&logFont);
	sampleFont.CreateFontIndirect(&logFont);
	GetDlgItem(IDC_FONT_SAMPLE_1)->SetFont(&sampleFont);
	GetDlgItem(IDC_FONT_SAMPLE_2)->SetFont(&sampleFont);

	// 色変更用ブラシ
	m_brDlg.CreateSolidBrush(parentDlg->iniData.dlgColor);
	m_fontColorN = parentDlg->iniData.fntColorN;
	m_fontColorR = parentDlg->iniData.fntColorR;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// 適用またはOK時にINIデータを更新する
void CPropFont::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (fontInfo.fontName.GetLength() > 0)
	{
		parentDlg->iniData.fontInfo = fontInfo;
		parentDlg->openStatusWindow();
	}

	CMFCPropertyPage::OnOK();
}

// 終了処理
void CPropFont::OnDestroy()
{
	CMFCPropertyPage::OnDestroy();

	// 後始末
	sampleFont.DeleteObject();
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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CFontDialog fontDlg;
	fontDlg.m_cf.lpLogFont = &logFont;
	fontDlg.m_cf.Flags ^= CF_EFFECTS;
	fontDlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;

	if (fontDlg.DoModal() == IDOK)
	{
		fontInfo.fontName = fontDlg.GetFaceName();
		fontInfo.fontSize = fontDlg.GetSize()/10;
		fontInfo.fontWeight = fontDlg.GetWeight();
		fontInfo.fontItalic = fontDlg.IsItalic();

		fontDlg.GetCurrentFont(&logFont);
				
		sampleFont.DeleteObject();
		sampleFont.CreateFontIndirect(&logFont);

		GetDlgItem(IDC_FONT_SAMPLE_1)->SetFont(&sampleFont);
		GetDlgItem(IDC_FONT_SAMPLE_2)->SetFont(&sampleFont);
	}
}



