#pragma once

#include <afx.h>
#include <vector>

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
	CString string;
	ULONGLONG offset;
};


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
	long long getCurrentMeseta(CString path);

	CString lastRead;
	ULONGLONG lastPoint;
};
