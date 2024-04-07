// INI�ɕۑ�����f�[�^�܂Ƃ�
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
	CString ngs_log_path; // log_ngs�t�H���_
	BOOL pos_save; // �E�B���h�E�ʒu�L��
	BOOL disp_interval; // �C���^�[�o���\��
	BOOL auto_refresh; // �����X�V
	int refresh_second; // �����X�V�Ԋu
	COLORREF dlgColor; // �_�C�A���O�w�i�F
	COLORREF fntColorN; // �����ʏ�J���[
	COLORREF fntColorR; // �����L�^���J���[
	BOOL hotkeyUseDell; // �폜�L�[�̎g�p
	CString hotkeyRec; // �L�^�L�[
	CString hotkeyDel; // �폜�L�[
	BOOL padUse; // �p�b�h�g�p
	CString padRec; // �L�^�{�^��
	CString padDel; // �폜�{�^��
	int padNum; // �p�b�hID
	CString saveDirectory; // �ۑ��t�H���_
};
