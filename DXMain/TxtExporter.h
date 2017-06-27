#pragma once
#include "FileExporter.h"

//File Exporter가 사용하는 객체
class CTxtExporter :public CFileExporter{
public:
	void WriteWstring(wofstream& out, const wstring data);
	//void WriteCHAR(wofstream& out, const char* data, int size);
	void WriteUINT(wofstream& out, const UINT data);
	void WriteInt(wofstream& out, const int data);
	void WriteFloat(wofstream& out, const float data);

	//txt file 사용할 경우 각 인자의 경계
	void WriteSpace(wofstream& out);
	void WriteEnter(wofstream& out);
	void WriteBool(wofstream& out, const bool b);
private:
	
public:
	CTxtExporter();
	~CTxtExporter();
};
