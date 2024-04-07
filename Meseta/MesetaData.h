// ���Z�^�f�[�^�̊Ǘ��p�N���X
#pragma once

#include <afx.h>
#include <vector>

#include "EnumFile.h"
#include "ReadText.h"


// ��񕪂̃��O�L�^�f�[�^
class MesetaData
{
public:
	MesetaData()
		: meseta(0)
		, mps(0)
		, idx(0)
		, recTime(0)
		, interval(0)
	{}

	void clear()
	{
		idx = 0;
		meseta = 0;
		mps = 0;
		recTime = 0;
		interval = 0;
	}

	long long calcMPS()
	{
		if (recTime==0)
			recTime = end.GetTime() - start.GetTime();

		mps = (recTime==0) ? meseta : (long long)((long double)meseta / (long double)recTime);
		return mps;
	}

	int idx;
	long long meseta;
	long long mps;
	CTime start;
	CTime end;
	long long recTime;
	long long interval;
};

// �L�^���̃f�[�^
class CurrentMesetaData : public MesetaData
{
public:
	CurrentMesetaData()
		: isRun(false)
		, startMesta(0)
		, endMeseta(0)
		, auto_count(0)
	{}
	bool isRun;
	long long startMesta;
	long long endMeseta;
	int auto_count;

	void clear()
	{
		isRun = false;
		startMesta = 0;
		endMeseta = 0;
		auto_count = 0;
		MesetaData::clear();
	}
};

// �f�[�^�Ǘ��N���X
class MesetaDataCtrl
{
public:
	MesetaDataCtrl();
	void clear();

	bool Init(CString path, bool run = false);

	long long getCurrentMeseta(bool init=false);
	void setCurrentData(long long meseta, int auto_count = 0);
	void endCurrentData(long long meseta);
	MesetaData pushCurrentData();

	bool writeData();

	void pop() { mesetaData.pop_back(); }
	bool isRunning() { return isActive; }

	CString getElapsedTime(CTime t);
	
public:
	CEnumFile enumFile;
	CReadCurrentMesetaF readMeseta;

	CTime initialTime;
	long long elapsedTime;
	long long initialMeseta;
	long long increasedMeseta;
	long long mesetaPerHour;

	CurrentMesetaData currentMeseta;
	std::vector <MesetaData> mesetaData; 

	BOOL isActive;

public:
	// ���[�e�B���e�B
	static CString MakeBigNumber(long long num)
	{
		CString strNum;
		strNum.Format(L"%lld", num);
		CString output = L"";

		while (1)
		{
			CString buff = strNum.Right(3);
			strNum.Delete(strNum.GetLength() - buff.GetLength(), buff.GetLength());
			output = buff + output;
			if (strNum.GetLength() == 0) break;
			output.Insert(0, L",");
		}
		return output;
	}

	static CString Time2Min(long long time)
	{
		CString str;
		long long itMIn = (long long)((long double)time / 60.0);
		long long itSec = time % 60;

		str.Format(L"%02lld:%02lld", itMIn, itSec);

		return str;
	}
};