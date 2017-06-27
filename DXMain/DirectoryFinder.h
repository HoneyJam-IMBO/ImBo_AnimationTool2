#pragma once
#include "SingleTon.h"

// 유니코드 전용 샘플 코드
#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "shlwapi")
#include <Shlwapi.h>

#define STR_TXT(T) TEXT(T) ## s

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif

using namespace std;

class CDirectoryFinder : public CSingleTonBase<CDirectoryFinder> {
public:
	bool Begin(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	bool End();

	//bool 1. 모든 하위 디렉토리 조사/ 2. 모든 디렉토리 표시 
	void GetFiles(vector<tstring> &vList, tstring sPath, bool bSearchAllDirectories, bool bAllDirectories, WCHAR* sExtension = nullptr, int length = -1);
	std::wstring ReplaceString(std::wstring subject, const std::wstring &search, const std::wstring &replace);
private:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;

public:
	CDirectoryFinder() : CSingleTonBase<CDirectoryFinder>("directoryfindersingleton") {}

};
