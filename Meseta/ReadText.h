#pragma once

#include <afx.h>
#include <vector>


class CReadText
{
public:
	CReadText();
	~CReadText();

	bool ReadText(CString path);
	bool ReadTextUTF16(CString path);

	CString getLine(int line);

protected:
	std::vector<CString> data;
};



class CReadCurrentMeseta : public CReadText
{
public:
	long long getCurrentMeseta(CString path);
};
