#pragma once

#include <afx.h>
#include <vector>

// ファイル読み取り時の１行分のテキストデータ
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
	CString string; // 文字列
	ULONGLONG offset; // ファイルポインター
};

// ファイルからのテキスト読み取りクラス
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

// ログファイルからメセタを読み取り用クラス
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

	// メセタ読み取り
	long long getCurrentMeseta(CString path);

	// 最後に読み取ったファイルと行を記憶する
	CString lastRead;
	ULONGLONG lastPoint;
};
