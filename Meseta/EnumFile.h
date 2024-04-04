#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include<atlstr.h>

class CFileInfo
{
public:
	CString file;
	int lastWrite;


	static bool compare(const CFileInfo& a, const CFileInfo& b) { return a.lastWrite > b.lastWrite; }
};

class CEnumFile
{
public:
	CEnumFile();
	~CEnumFile() {}

	void clear();

	bool enumFile(CString path, const char* ext, const char* search = "" );
	CString getFile(size_t idx) { return (files[idx].file); }

	size_t getSize() { return files.size(); }

private:
	std::vector<CFileInfo> files;
};