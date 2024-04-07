#include <locale.h>

#include "ReadText.h"


CReadText::CReadText()
{
}

CReadText::~CReadText()
{
	data.clear();
}

// �����R�[�h��UTF-8����UTF-16�ɕϊ�����
CStringW __stdcall _Utf8ToUtf16(const char* utf8)
{
    int nSize = ::MultiByteToWideChar(CP_UTF7, 0, utf8, -1, NULL, 0);    //�ϊ���̕������𒲂ׂ܂��B
    CStringW strW;                      //�ϊ���̕������i�[����CStringW�I�u�W�F�N�g��p�ӂ��܂��B
    ::MultiByteToWideChar(CP_UTF7, 0, utf8, -1, strW.GetBufferSetLength(nSize), nSize);  //�ϊ����܂��B
    strW.ReleaseBuffer();                               //�]���ȋL����i�k�������̕��j���J�����܂��B
    return strW;
}

// �e�L�X�g�t�@�C�����P�s���ǂݎ��iUTF-8�j
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
        //CString read;
        while (1)
        {
            CFileData fd(file.GetPosition());
            if (!file.ReadString(fd.string))
                break;

            // �f�[�^��ǂݍ��߂��ꍇ
            data.push_back(fd);
        }
        // �e�L�X�g�t�@�C�����N���[�Y���܂��B
        file.Close();
    }
    else
        return false;

	return true;
}

// �e�L�X�g�t�@�C�����P�s���ǂݎ��iUTF-16�j
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
        // �e�L�X�g�t�@�C�����I�[�v���ł����ꍇ
        // �e�L�X�g�t�@�C���̃f�[�^��1�s���ǂݍ��݂܂��B
        // �f�[�^������ԌJ��Ԃ��܂��B
        while (1)
        {
            CFileData fd(file.GetPosition());
            if (!file.ReadString(fd.string))
                break;

            // �f�[�^��ǂݍ��߂��ꍇ
            data.push_back(fd);
        }
        // �e�L�X�g�t�@�C�����N���[�Y���܂��B
        file.Close();
    }
    else
        return false;

    return true;
}

// �ǂݎ�����e�L�X�g�̍s�w��œǂݎ��
CString CReadText::getLine(int line)
{
	if (line < data.size())
		return data[line].string;

	return "";
}

// ���݂̃��Z�^�����O�t�@�C������ǂݎ��
long long CReadCurrentMesetaF::getCurrentMeseta(CString path)
{
    long long total = -1;
    const CString TAG = L"CurrentN-Meseta(";

    // ���ڃt�@�C���I�[�v���ł��Ȃ��̂ň�U�R�s�[����
    // ActionLog�t�@�C������債�Ă�ꍇ�V�X�e���ɕ��ׂ�������\��������
    // �Ȃɂ��A�C�f�A��W��
    if (!CopyFile(path, L"log.tmp", false)) {
        return total;
    }

    // �ȑO�ɓǂݎ�����t�@�C���Ɠ����������ꍇ�Ō�ɓǂݎ�����s����ĊJ����
    ULONGLONG seek = 0;
    if (path == lastRead)
        seek = lastPoint;

    // �ǂݎ��J�n
    if (ReadTextUTF16(L"log.tmp", seek))
    {
        while (data.size() > 0)
        {
            CString text = data.back().string;

            // ���Z�^�ύX���O���`�F�b�N
            int tagPoint = text.Find(TAG);
            if (tagPoint == -1)
            {
                data.pop_back();
                continue;
            }

            // ���Z�^�Ɋ֌W���郍�O�������ꍇ�͓ǂݎ��
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

