// INIに保存するデータまとめ
#pragma once

#include "framework.h"

class FONT_INFO
{
public:
	FONT_INFO()
		: fontSize(0)
		, fontWeight(400)
		, fontItalic(0)
	{}
	CString fontName; // フォント
	WORD fontSize; // フォントサイズ
	LONG fontWeight; // フォント重み
	BYTE fontItalic; // フォント斜体
};

class INI_FILE
{
public:
	INI_FILE()
		: pos_save(false)
		, disp_interval(false)
		, auto_refresh(false)
		, refresh_second(10)
		, dlgColor(0)
		, fntColorN(0)
		, fntColorR(0)
		, hotkeyUseDell(false)
		, padUse(false)
		, padNum(0)
	{}
	CString ngs_log_path; // log_ngsフォルダ
	BOOL pos_save; // ウィンドウ位置記憶
	BOOL disp_interval; // インターバル表示
	BOOL auto_refresh; // 自動更新
	int refresh_second; // 自動更新間隔
	COLORREF dlgColor; // ダイアログ背景色
	COLORREF fntColorN; // 文字通常カラー
	COLORREF fntColorR; // 文字記録時カラー
	BOOL hotkeyUseDell; // 削除キーの使用
	CString hotkeyRec; // 記録キー
	CString hotkeyDel; // 削除キー
	BOOL padUse; // パッド使用
	CString padRec; // 記録ボタン
	CString padDel; // 削除ボタン
	int padNum; // パッドID
	CString saveDirectory; // 保存フォルダ
	FONT_INFO fontInfo; // フォント
};
