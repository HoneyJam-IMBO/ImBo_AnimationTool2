#pragma once
#include "FileImporter.h"

//File Exporter가 사용하는 객체
class CTxtImporter :public CFileImporter {
public:
	virtual string Readstring(ifstream& in);
	virtual UINT ReadUINT(ifstream& in);
	virtual int ReadInt(ifstream& in);
	virtual float ReadFloat(ifstream& in);
	virtual bool ReadBool(ifstream& in);

	//txt file 사용할 경우 각 인자의 경계
	virtual void ReadSpace(ifstream& in);
	virtual void ReadEnter(ifstream& in);
private:

public:
	CTxtImporter();
	~CTxtImporter();
};
