#pragma once
//#include "Camera.h"
#include "SingleTon.h"
#include "FileImporter.h"
#include "TxtImporter.h"
#include <fstream>

class CImporter : public CSingleTonBase<CImporter> {
public:
	bool Begin(string inputPath);//file open하구
	virtual bool End();//file close

	//const WCHAR* ReadWCHAR();
	//const char* ReadCHAR();
	const wstring ReadWstring();
	const string Readstring();
	UINT ReadUINT();
	int ReadInt();
	float ReadFloat();
	XMFLOAT4X4 ReadFloat4x4();
	XMMATRIX ReadXMMatrix();
	bool ReadBool();
	//txt file 사용할 경우 각 인자의 경계
	void ReadSpace();
	void ReadEnter();
private:
	string m_inputPath;
	ifstream m_in;
	
	CFileImporter* m_pFileImporter{ nullptr };
public:
	CImporter() : CSingleTonBase<CImporter>("importersingleton") {}

};
