// enumFile
// log_ngs�t�H���_���𑖍�����ActionLog�t�@�C����񋓂���
//
#include "framework.h"
#include "EnumFile.h"

using namespace std;
namespace fs = std::filesystem;

// �R���X�g���N�^(�������̂�)
CEnumFile::CEnumFile()
{
	clear();
}

// �񋓍σt�@�C�����N���A
void CEnumFile::clear()
{
	files.clear();
}

// NGS�̃��O�t�H���_�𑖍�����ActionLog�t�@�C����񋓂���
bool CEnumFile::enumFile(CString path, const char* ext, const char* search)
{
	files.clear();

	// �p�X�Ɉȉ��̊��ϐ����܂܂��ꍇ�͒u������
	// �����N���Ɏ��s���Ȃ�����K�v�͂Ȃ��͂������O�̂���
	CString user[2] = { L"%UserProfile%", L"%OneDrive%" };
	for (int i = 0; i < 2; i++)
	{
		int up = path.Find(user[i]);
		if (up >= 0)
		{
			path.Delete(0, user[i].GetLength());
			user[i].Replace(L"%", L"");
			if (user[i].GetEnvironmentVariable(user[i]) == 0)
				return false;
			path = user[i] + path;
		}
	}

	// �w�肳�ꂽ�p�X
	fs::path input = path.GetString(); // ���̓t�H���_

	// �t�H���_�����݂��Ȃ�������s���I��
	if (!fs::exists(input))
	{
		return false;
	}

	fs::directory_iterator iter(input), end;
	std::error_code err;
	
	// �t�@�C������ActionLog���܂܂��t�@�C��������
	CString searchFile(search);

	for (; iter != end && !err; iter.increment(err)) {
		const fs::directory_entry entry = *iter;

		CString fileName(entry.path().filename().c_str());
		if (searchFile.GetLength() == 0 || fileName.Find(searchFile) != -1)
		{
			// �g���q��.txt ���� �t�@�C������ActionLog���܂܂��ꍇ�̂ݗ�
			if (entry.path().extension().compare(ext) == 0)
			{
				CFileInfo info;
				info.file = entry.path().c_str();

				CString lastWrite = info.file;
				int pos = lastWrite.Find(L"ActionLog");
				lastWrite.Delete(0, pos);
				lastWrite.Replace(L"ActionLog", L"");
				lastWrite.Replace(L"_", L"");
				lastWrite.Replace(L".txt", L"");
				info.lastWrite = _ttoi(lastWrite);				
				files.push_back(info);
			}
		}
	}	

	// 1��������Ȃ�������s���I��
	if (files.size() == 0)
		return false;
	
	// �X�V�����̐V�������Ƀ\�[�g����
	// Windows�̕W���̓���ł̓t�@�C�������ɗ񋓂����̂ŕK�v�Ȃ��͂������O�̂���
	std::sort(files.begin(), files.end(), CFileInfo::compare);

	return true;
}