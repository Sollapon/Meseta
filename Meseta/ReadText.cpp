#include <locale.h>

#include "ReadText.h"


CReadText::CReadText()
{
}


CReadText::~CReadText()
{
	data.clear();
}


CStringW __stdcall _Utf8ToUtf16(const char* utf8)
{
    int nSize = ::MultiByteToWideChar(CP_UTF7, 0, utf8, -1, NULL, 0);    //変換後の文字数を調べます。
    CStringW strW;                      //変換後の文字を格納するCStringWオブジェクトを用意します。
    ::MultiByteToWideChar(CP_UTF7, 0, utf8, -1, strW.GetBufferSetLength(nSize), nSize);  //変換します。
    strW.ReleaseBuffer();                               //余分な記憶域（ヌル文字の分）を開放します。
    return strW;
}

bool CReadText::ReadText(CString path)
{
	data.clear();

	//setlocale(LC_ALL, "jpn");
    CString fileName = path;
    UINT nOpenFlags = CFile::modeRead;
    FILE* fp;
    _wfopen_s(&fp, fileName, _T("r, ccs=UTF-8"));

    CStdioFile file(fp);
    if (file)
    {
        // テキストファイルがオープンできた場合
        // テキストファイルのデータを1行ずつ読み込みます。
        // データがある間繰り返します。
        //CString read;
        while (1)
        {
            CFileData fd(file.GetPosition());
            if (!file.ReadString(fd.string))
                break;

            // データを読み込めた場合
            data.push_back(fd);
        }
        // テキストファイルをクローズします。
        file.Close();
    }
    else
        return false;

	return true;
}

bool CReadText::ReadTextUTF16(CString path, ULONGLONG offset)
{
    data.clear();

    CString fileName = path;
    UINT nOpenFlags = CFile::modeRead;
    FILE* fp;
    _wfopen_s(&fp, fileName, _T("r, ccs=UTF-16LE"));

    CStdioFile file(fp);
    file.Seek(offset, CFile::begin);
    if (file)
    {
        // テキストファイルがオープンできた場合
        // テキストファイルのデータを1行ずつ読み込みます。
        // データがある間繰り返します。
        while (1)
        {
            CFileData fd(file.GetPosition());
            if (!file.ReadString(fd.string))
                break;

            // データを読み込めた場合
            data.push_back(fd);
        }
        // テキストファイルをクローズします。
        file.Close();
    }
    else
        return false;

    return true;
}

CString CReadText::getLine(int line)
{
	if (line < data.size())
		return data[line].string;

	return "";
}


long long CReadCurrentMesetaF::getCurrentMeseta(CString path)
{
    
    long long total = -1;
    const CString TAG = L"CurrentN-Meseta(";


    if (!CopyFile(path, L"log.tmp", false)) {
        return total;
    }

    ULONGLONG seek = 0;
    if (path == lastRead)
        seek = lastPoint;

    if (ReadTextUTF16(L"log.tmp", seek))
    {
        while (data.size() > 0)
        {
            CString text = data.back().string;

            int tagPoint = text.Find(TAG);

            if (tagPoint == -1)
            {
                data.pop_back();
                continue;
            }

            text.Delete(0, tagPoint + CString(TAG).GetLength());

            int endPoint = text.Find(L")");
            text.Delete(endPoint, text.GetLength());
            text.Replace(L" ", L"");
            text.Replace(L",", L"");
            total = _ttoll(text);
            lastRead = path;
            lastPoint = data.back().offset;
            break;
        }
    }

    return total;
}

