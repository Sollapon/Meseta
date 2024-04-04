
#pragma once

#include "framework.h"

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
	CString ngs_log_path;
	BOOL pos_save;
	BOOL disp_interval;
	BOOL auto_refresh;
	int refresh_second;
	COLORREF dlgColor;
	COLORREF fntColorN;
	COLORREF fntColorR;

	BOOL hotkeyUseDell;
	CString hotkeyRec;
	CString hotkeyDel;

	BOOL padUse;
	CString padRec;
	CString padDel;
	int padNum;
};
