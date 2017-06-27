#pragma once
#include <fstream>

//File Exporter가 사용하는 객체
class CFileExporter {
public:
	virtual void WriteWstring(wofstream& out, wstring data) = 0;
	//virtual void WriteCHAR(wofstream& out, const char* data, int size) = 0;
	virtual void WriteUINT(wofstream& out, const UINT data) = 0;
	virtual void WriteInt(wofstream& out, const int data) = 0;
	virtual void WriteFloat(wofstream& out, const float data) = 0;
	virtual void WriteBool(wofstream& out, const bool b) = 0;

	//txt file 사용할 경우 각 인자의 경계
	virtual void WriteSpace(wofstream& out) = 0;
	virtual void WriteEnter(wofstream& out) = 0;
private:

public:
	CFileExporter() {};
	~CFileExporter() {};
};

