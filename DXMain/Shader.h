#pragma once
#include "DXObject.h"

class CShader : public DXObject{
public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();
	//----------------------------dxobject-----------------------------


protected:
	vector<byte> ReadCSOFile(LPCTSTR path)
	{
		ifstream inputShaderCSO(path, ios::in | ios::binary);

		if (!inputShaderCSO.is_open())
		{
			//MessageBox(m_hWnd, TEXT("Shader Load Fail!"), TEXT("프로그램 구동 실패"), MB_OK);
			return vector<byte>();
		}

		// size check;
		inputShaderCSO.seekg(0, fstream::end);
		size_t szData = inputShaderCSO.tellg();
		inputShaderCSO.seekg(0);

		vector<byte> retval;
		retval.resize(szData);

		inputShaderCSO.read(reinterpret_cast<char*>(&(retval[0])), szData);
		inputShaderCSO.close();

		return retval;
	}


public:
	CShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CShader();
};
