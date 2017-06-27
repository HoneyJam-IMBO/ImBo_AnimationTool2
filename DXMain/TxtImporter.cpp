#include "stdafx.h"
#include "TxtImporter.h"


#define TEXT_BUFF_SIZE 256
string CTxtImporter::Readstring(ifstream& in) {
	UINT size;
	in >> size;
	if (size > TEXT_BUFF_SIZE) return string("size overflow");
	char data[TEXT_BUFF_SIZE];
	in >> data;
	string ws{ data };
	return ws;
}

UINT CTxtImporter::ReadUINT(ifstream& in) {
	UINT data;
	in >> data;
	return data;
}

int CTxtImporter::ReadInt(ifstream& in) {
	int data;
	in >> data;
	return data;
}

float CTxtImporter::ReadFloat(ifstream& in) {
	float data;
	in >> data;
	return data;
}

bool CTxtImporter::ReadBool(ifstream & in){
	bool data;
	in >> data;
	return data;
}

void CTxtImporter::ReadSpace(ifstream& in) {
	//in >> L" ";
}

void CTxtImporter::ReadEnter(ifstream& in) {
	//in >> L"\n";
}

CTxtImporter::CTxtImporter() {
}

CTxtImporter::~CTxtImporter() {

}
