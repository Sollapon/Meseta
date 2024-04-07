#pragma once

#include <afx.h>
#include <vector>

// �t�@�C���ǂݎ�莞�̂P�s���̃e�L�X�g�f�[�^
class CFileData
{
public:
	CFileData()
		: string(L"")
		, offset(0)
	{

	}

	CFileData(ULONGLONG ofs)
		: string(L"")
		, offset(ofs)
	{

	}
	CString string; // ������
	ULONGLONG offset; // �t�@�C���|�C���^�[
};

// �t�@�C������̃e�L�X�g�ǂݎ��N���X
class CReadText
{
public:
	CReadText();
	~CReadText();

	bool ReadText(CString path);
	bool ReadTextUTF16(CString path, ULONGLONG offset =0 );

	CString getLine(int line);

protected:
	std::vector<CFileData> data;
};

// ���O�t�@�C�����烁�Z�^��ǂݎ��p�N���X
class CReadCurrentMesetaF : public CReadText
{
public:
	CReadCurrentMesetaF()
		:lastRead(L"")
		, lastPoint(0)
	{
	}

	void Clear()
	{
		lastRead = L"";
		lastPoint = 0;
	}

	// ���Z�^�ǂݎ��
	long long getCurrentMeseta(CString path);

	// �Ō�ɓǂݎ�����t�@�C���ƍs���L������
	CString lastRead;
	ULONGLONG lastPoint;
};
