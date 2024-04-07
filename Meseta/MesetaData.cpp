// MesetaDataCtrl
// メセタデータの制御クラス
// 
#include "MesetaData.h"

// コンストラクタ
// 初期化のみ
MesetaDataCtrl::MesetaDataCtrl()
	: initialMeseta(0)
	, increasedMeseta(0)
	, mesetaPerHour(0)
	, elapsedTime(0)
	, isActive(false)
{

}

// リセット（実行終了)
void MesetaDataCtrl::clear()
{
	mesetaData.clear();
	currentMeseta.clear();
	isActive = false;
}

// 初期化
// CString path：log_ngsのパス
//bool run：実行あり true、実行無し false
bool MesetaDataCtrl::Init(CString path, bool run)
{
	// 起動時の時間を記録する
	initialTime = CTime::GetCurrentTime();
	elapsedTime = 0;
	increasedMeseta = 0;
	mesetaPerHour = 0;

	// 諸々初期化
	readMeseta.Clear(); // テキスト読み取りクラス
	mesetaData.clear(); // ログデータ
	currentMeseta.clear(); // 記録中データ

	// ngs_logフォルダからActionLogファイルを列挙する
	enumFile.clear();
	bool ret = enumFile.enumFile(path, ".txt", "ActionLog");
	if (!ret)
		return false; // ActionLogファイルが一個もない

	// 起動時のメセタ記録
	initialMeseta = getCurrentMeseta(true);
	if (initialMeseta < 0)
	{
		initialMeseta = 0;
		return false; // 一度もメセタを取得してない
	}	

	// 実行
	isActive = run;

	return true;
}

// 現在のメセタ読み取り
long long MesetaDataCtrl::getCurrentMeseta(bool init)
{
	long long m = -1;

	// 列挙済みログファイルを最新順に操作する
	for (size_t i = 0; i < enumFile.getSize(); i++)
	{
		CString file = enumFile.getFile(i);

		// メセタログが見つかり次第終了
		m = readMeseta.getCurrentMeseta(file);
		if (m >= 0)
			break;
	}

	// メセタの増加量とMPHを計算
	if (!init && m >= 0)
	{
		increasedMeseta = m - initialMeseta;
		long double second = (long double)elapsedTime / 3600.0;
		mesetaPerHour = (second > 0) ? (long long)((long double)increasedMeseta / second) : 0;
	}

	// 現在のメセタを返す
	return m;
}

// 記録データを開始値をセット
void MesetaDataCtrl::setCurrentData(long long meseta, int auto_count)
{
	currentMeseta.clear();
	currentMeseta.start = CTime::GetCurrentTime();
	currentMeseta.isRun = true;
	currentMeseta.startMesta = meseta;
	currentMeseta.auto_count = auto_count;
	currentMeseta.recTime = auto_count;
}

// 記録データの終了値
void MesetaDataCtrl::endCurrentData(long long meseta)
{
	currentMeseta.end = CTime::GetCurrentTime();
	currentMeseta.endMeseta = meseta;
	currentMeseta.meseta = currentMeseta.endMeseta - currentMeseta.startMesta;
	currentMeseta.calcMPS();
	currentMeseta.isRun = false;
	currentMeseta.auto_count = 0;
}

// ログデータをCSVファイルに書き出す
bool MesetaDataCtrl::writeData()
{
	isActive = false;

	// 現在時刻をファイル名にする
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

