// enumFile
// log_ngsフォルダ内を走査してActionLogファイルを列挙する
//
#include "framework.h"
#include "EnumFile.h"

using namespace std;
namespace fs = std::filesystem;

// コンストラクタ(初期化のみ)
CEnumFile::CEnumFile()
{
	clear();
}

// 列挙済ファイルをクリア
void CEnumFile::clear()
{
	files.clear();
}

// NGSのログフォルダを走査してActionLogファイルを列挙する
bool CEnumFile::enumFile(CString path, const char* ext, const char* search)
{
	files.clear();

	// パスに以下の環境変数が含まれる場合は置き換え
	// 初期起動に失敗しない限り必要はないはずだが念のため
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

	// 指定されたパス
	fs::path input = path.GetString(); // 入力フォルダ

	// フォルダが存在しなかったら不正終了
	if (!fs::exists(input))
	{
		return false;
	}

	fs::directory_iterator iter(input), end;
	std::error_code err;
	
	// ファイル名にActionLogが含まれるファイルを検索
	CString searchFile(search);

	for (; iter != end && !err; iter.increment(err)) {
		const fs::directory_entry entry = *iter;

		CString fileName(entry.path().filename().c_str());
		if (searchFile.GetLength() == 0 || fileName.Find(searchFile) != -1)
		{
			// 拡張子が.txt 且つ ファイル名にActionLogが含まれる場合のみ列挙
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

	// 1つも見つからなかったら不正終了
	if (files.size() == 0)
		return false;
	
	// 更新日時の新しい順にソートする
	// Windowsの標準の動作ではファイル名順に列挙されるので必要ないはずだが念のため
	std::sort(files.begin(), files.end(), CFileInfo::compare);

	return true;
}