// MesetaDataCtrl
// ���Z�^�f�[�^�̐���N���X
// 
#include "MesetaData.h"

// �R���X�g���N�^
// �������̂�
MesetaDataCtrl::MesetaDataCtrl()
	: initialMeseta(0)
	, increasedMeseta(0)
	, mesetaPerHour(0)
	, elapsedTime(0)
	, isActive(false)
{

}

// ���Z�b�g�i���s�I��)
void MesetaDataCtrl::clear()
{
	mesetaData.clear();
	currentMeseta.clear();
	isActive = false;
}

// ������
// CString path�Flog_ngs�̃p�X
//bool run�F���s���� true�A���s���� false
bool MesetaDataCtrl::Init(CString path, bool run)
{
	// �N�����̎��Ԃ��L�^����
	initialTime = CTime::GetCurrentTime();
	elapsedTime = 0;
	increasedMeseta = 0;
	mesetaPerHour = 0;

	// ���X������
	readMeseta.Clear(); // �e�L�X�g�ǂݎ��N���X
	mesetaData.clear(); // ���O�f�[�^
	currentMeseta.clear(); // �L�^���f�[�^

	// ngs_log�t�H���_����ActionLog�t�@�C����񋓂���
	enumFile.clear();
	bool ret = enumFile.enumFile(path, ".txt", "ActionLog");
	if (!ret)
		return false; // ActionLog�t�@�C��������Ȃ�

	// �N�����̃��Z�^�L�^
	initialMeseta = getCurrentMeseta(true);
	if (initialMeseta < 0)
	{
		initialMeseta = 0;
		return false; // ��x�����Z�^���擾���ĂȂ�
	}	

	// ���s
	isActive = run;

	return true;
}

// ���݂̃��Z�^�ǂݎ��
long long MesetaDataCtrl::getCurrentMeseta(bool init)
{
	long long m = -1;

	// �񋓍ς݃��O�t�@�C�����ŐV���ɑ��삷��
	for (size_t i = 0; i < enumFile.getSize(); i++)
	{
		CString file = enumFile.getFile(i);

		// ���Z�^���O�������莟��I��
		m = readMeseta.getCurrentMeseta(file);
		if (m >= 0)
			break;
	}

	// ���Z�^�̑����ʂ�MPH���v�Z
	if (!init && m >= 0)
	{
		increasedMeseta = m - initialMeseta;
		long double second = (long double)elapsedTime / 3600.0;
		mesetaPerHour = (second > 0) ? (long long)((long double)increasedMeseta / second) : 0;
	}

	// ���݂̃��Z�^��Ԃ�
	return m;
}

// �L�^�f�[�^���J�n�l���Z�b�g
void MesetaDataCtrl::setCurrentData(long long meseta, int auto_count)
{
	currentMeseta.clear();
	currentMeseta.start = CTime::GetCurrentTime();
	currentMeseta.isRun = true;
	currentMeseta.startMesta = meseta;
	currentMeseta.auto_count = auto_count;
	currentMeseta.recTime = auto_count;
}

// �L�^�f�[�^�̏I���l
void MesetaDataCtrl::endCurrentData(long long meseta)
{
	currentMeseta.end = CTime::GetCurrentTime();
	currentMeseta.endMeseta = meseta;
	currentMeseta.meseta = currentMeseta.endMeseta - currentMeseta.startMesta;
	currentMeseta.calcMPS();
	currentMeseta.isRun = false;
	currentMeseta.auto_count = 0;
}

// ���O�f�[�^��CSV�t�@�C���ɏ����o��
bool MesetaDataCtrl::writeData()
{
	isActive = false;

	// ���ݎ������t�@�C�����ɂ���
	CTime cTime = CTime::GetCurrentTime();           // ���ݎ���
	CString fileName = cTime.Format("%Y-%m%d-%H%M%S.csv");
	FILE* fp;
	_wfopen_s(&fp, fileName, _T("w, ccs=UTF-8"));
	CStdioFile file(fp);
	if (file)
	{
		CString write;

		file.WriteString(L",\n");

		write.Format(L",�v���C����,%s\n",getElapsedTime(cTime).GetString());
		file.WriteString(write);

		write.Format(L",�擾���Z�^,%lld\n", increasedMeseta);
		file.WriteString(write);

		write.Format(L",����,%lld\n", mesetaPerHour);
		file.WriteString(write);

		file.WriteString(L",\n");

		write.Format(L"id, ���Z�^, �J�n����, �L�^����, MPS, �Ԋu\n");
		file.WriteString(write);		

		for (size_t i = 0; i < mesetaData.size(); i++)
		{
			MesetaData& ref = mesetaData[i];
			CString day = ref.start.Format(L"%H:%M:%S");
			CString interval = MesetaDataCtrl::Time2Min(ref.interval);
			
			write.Format(L"%d, %lld, %s, %lld�b, %lld/s, %s\n", ref.idx, ref.meseta, day.GetString(), ref.recTime, ref.mps, interval.GetString());
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

