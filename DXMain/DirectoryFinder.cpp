#include "stdafx.h"
#include "DirectoryFinder.h"

bool CDirectoryFinder::Begin(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext) {
	m_pd3dDevice = pd3dDevice;
	m_pd3dDeviceContext = pd3dDeviceContext;

	return true;
}

bool CDirectoryFinder::End() {

	return true;
}

void CDirectoryFinder::GetFiles(vector<tstring>& vList, tstring sPath, bool bSearchAllDirectories, bool bAllDirectories, WCHAR* sExtension,int length) {
	tstring sTmp = sPath + tstring(TEXT("\\*.*"));


	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(sTmp.c_str(), &fd);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bSearchAllDirectories)
				{
					if (fd.cFileName[0] != '.')
					{
						sTmp = sPath
							+ STR_TXT("\\")
							+ tstring(fd.cFileName);
						GetFiles(vList, sTmp, bSearchAllDirectories, bAllDirectories, sExtension,(length < 0 ? (sPath + STR_TXT("\\")).length() : length));
					}
				}
			}
			else
			{
				//이부분에서 원하는 확장자만 남길 수 있음
				//PathFindExtension - 확장자의 위치를 찾아서 반환한다.
				//	"c:\abc\def.txt" -> ".txt"
				sTmp = sPath
					+ STR_TXT("\\")
					+ tstring(fd.cFileName);

				if (nullptr != sExtension) {//확장자 제한이 nullptr이 아니라면
					wstring fileName{ PathFindExtension(fd.cFileName) };
					if (fileName != sExtension){
						//해당 확장자가 아니면
						continue;
					}
				}
				
				if (length < 0) length = sPath.length();
				wstring s = (bAllDirectories ? sTmp : sTmp.substr(length, sTmp.length() - length));
				//s = ReplaceString(s, L"\\", L"/");
				vList.push_back(s);
			}

		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}

}

std::wstring CDirectoryFinder::ReplaceString(std::wstring subject, const std::wstring &search, const std::wstring &replace){
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::wstring::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}
