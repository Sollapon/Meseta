#include "framework.h"
#include "EnumFile.h"

using namespace std;
namespace fs = std::filesystem;


CEnumFile::CEnumFile()
{
	clear();
}


void CEnumFile::clear()
{
	files.clear();
}


bool CEnumFile::enumFile(CString path, const char* ext, const char* search)
{
	files.clear();

	// ユーザーフォルダーをパスに置き換え
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

	fs::path input = path.GetString(); // 入力フォルダ

	if (!fs::exists(input))
	{
		return false;
	}

	fs::directory_iterator iter(input), end;
	std::error_code err;
	
	CString searchFile(search);

	// 入力フォルダ内にあるファイルをtext_filesリストに追加
	//vector<fs::path> text_files;
	for (; iter != end && !err; iter.increment(err)) {
		const fs::directory_entry entry = *iter;

		CString fileName(entry.path().filename().c_str());
		if (searchFile.GetLength() == 0 || fileName.Find(searchFile) != -1)
		{
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

	if (files.size() == 0)
		return false;

	std::sort(files.begin(), files.end(), CFileInfo::compare);

	return true;
}