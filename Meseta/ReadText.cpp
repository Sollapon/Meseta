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
    int nSize = ::MultiByteToWideChar(CP_UTF7, 0, utf8, -1, NULL, 0);    //�ϊ���̕������𒲂ׂ܂��B
    CStringW strW;                      //�ϊ���̕������i�[����CStringW�I�u�W�F�N�g��p�ӂ��܂��B
    ::MultiByteToWideChar(CP_UTF7, 0, utf8, -1, strW.GetBufferSetLength(nSize), nSize);  //�ϊ����܂��B
    strW.ReleaseBuffer();                               //�]���ȋL����i�k�������̕��j���J�����܂��B
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
        // �e�L�X�g�t�@�C�����I�[�v���ł����ꍇ
        // �e�L�X�g�t�@�C���̃f�[�^��1�s���ǂݍ��݂܂��B
        // �f�[�^������ԌJ��Ԃ��܂��B
        CString read;
        while (file.ReadString(read))
        {
            // �f�[�^��ǂݍ��߂��ꍇ
            data.push_back(read);
        }
        // �e�L�X�g�t�@�C�����N���[�Y���܂��B
        file.Close();
    }
    else
        return false;

	return true;
}

bool CReadText::ReadTextUTF16(CString path)
{
    data.clear();

    CString fileName = path;
    UINT nOpenFlags = CFile::modeRead;
    FILE* fp;
    _wfopen_s(&fp, fileName, _T("r, ccs=UTF-16LE"));

    CStdioFile file(fp);
    if (file)
    {
        // �e�L�X�g�t�@�C�����I�[�v���ł����ꍇ
        // �e�L�X�g�t�@�C���̃f�[�^��1�s���ǂݍ��݂܂��B
        // �f�[�^������ԌJ��Ԃ��܂��B
        CString read;
        while (file.ReadString(read))
        {
            // �f�[�^��ǂݍ��߂��ꍇ
            data.push_back(read);
        }
        // �e�L�X�g�t�@�C�����N���[�Y���܂��B
        file.Close();
    }
    else
        return false;

    return true;
}

CString CReadText::getLine(int line)
{
	if (line < data.size())
		return data[line];

	return "";
}


long long CReadCurrentMeseta::getCurrentMeseta(CString path)
{

    long long total = -1;
    const CString TAG = L"CurrentN-Meseta(";

    if (ReadTextUTF16(path))
    {
        while (data.size() > 0)
        {
            CString text = data.back();

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
            break;
        }
    }

    return total;
}

