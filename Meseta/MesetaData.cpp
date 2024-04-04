#include "MesetaData.h"



MesetaDataCtrl::MesetaDataCtrl()
	: initialMeseta(0)
	, increasedMeseta(0)
	, mesetaPerHour(0)
	, elapsedTime(0)
	, isActive(false)
{

}


bool MesetaDataCtrl::Init(CString path, bool run)
{
	initialTime = CTime::GetCurrentTime();
	elapsedTime = 0;
	increasedMeseta = 0;
	mesetaPerHour = 0;

	readMeseta.Clear();
	mesetaData.clear();
	currentMeseta.clear();	

	enumFile.clear();
	bool ret = enumFile.enumFile(path, ".txt", "ActionLog");
	if (!ret)
		return false; // ActionLogファイルが一個もない

	initialMeseta = getCurrentMeseta(true);
	if (initialMeseta < 0)
	{
		initialMeseta = 0;
		return false; // 一度もメセタを取得してない
	}	

	isActive = run;

	return true;
}


bool MesetaDataCtrl::writeData()
{
	isActive = false;

	CTime cTime = CTime::GetCurrentTime();           // 現在時刻
	CString fileName = cTime.Format("%Y-%m%d-%H%M%S.csv");
	FILE* fp;
	_wfopen_s(&fp, fileName, _T("w, ccs=UTF-8"));
	CStdioFile file(fp);
	if (file)
	{
		CString write;

		file.WriteString(L",\n");

		write.Format(L",プレイ時間,%s\n",getElapsedTime(cTime).GetString());
		file.WriteString(write);

		write.Format(L",取得メセタ,%lld\n", increasedMeseta);
		file.WriteString(write);

		write.Format(L",時給,%lld\n", mesetaPerHour);
		file.WriteString(write);

		file.WriteString(L",\n");

		write.Format(L"id, メセタ, 開始時刻, 記録時間, MPS, 間隔\n");
		file.WriteString(write);		

		for (size_t i = 0; i < mesetaData.size(); i++)
		{
			MesetaData& ref = mesetaData[i];
			CString day = ref.start.Format(L"%H:%M:%S");
			CString interval = MesetaDataCtrl::Time2Min(ref.interval);
			
			write.Format(L"%d, %lld, %s, %lld秒, %lld/s, %s\n", ref.idx, ref.meseta, day.GetString(), ref.recTime, ref.mps, interval.GetString());
			file.WriteString(write);
		}

		/*
		file.WriteString(L",\n");
		write.Format(L",時給,%lld\n", mesetaPerHour);
		file.WriteString(write);
		*/

		file.Close();
	}
	else
		return false;

	return true;
}


CString MesetaDataCtrl::getElapsedTime(CTime t)
{
	elapsedTime = t.GetTime() - initialTime.GetTime();
	return (t-initialTime).Format(L"%H:%M:%S");
}


long long MesetaDataCtrl::getCurrentMeseta(bool init)
{
	long long m = -1;

	for (size_t i = 0; i < enumFile.getSize(); i++)
	{
		CString file = enumFile.getFile(i);

		//
		m = readMeseta.getCurrentMeseta(file);
		if (m >= 0)
			break;
	}

	if (!init && m >= 0)
	{
		increasedMeseta = m - initialMeseta;
		long double second = (long double)elapsedTime / 3600.0;
		mesetaPerHour = (second > 0) ? (long long)((long double)increasedMeseta / second) : 0;
	}

	return m;
}

void MesetaDataCtrl::clear()
{
	mesetaData.clear();
	currentMeseta.clear();
	isActive = false;
}


void MesetaDataCtrl::setCurrentData(long long meseta, int auto_count)
{
	currentMeseta.clear();
	currentMeseta.start = CTime::GetCurrentTime();
	currentMeseta.isRun = true;
	currentMeseta.startMesta = meseta;
	currentMeseta.auto_count = auto_count;
	currentMeseta.recTime = auto_count;
}


void MesetaDataCtrl::endCurrentData(long long meseta)
{
	currentMeseta.end = CTime::GetCurrentTime();
	currentMeseta.endMeseta = meseta;
	currentMeseta.meseta = currentMeseta.endMeseta - currentMeseta.startMesta;
	currentMeseta.calcMPS();
	currentMeseta.isRun = false;
	currentMeseta.auto_count = 0;
}