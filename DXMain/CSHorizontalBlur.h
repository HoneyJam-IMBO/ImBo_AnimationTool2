#pragma once
#include "ComputeShader.h"

class CCSHorizontalBlur : public CComputeShader {
public:
	//---------------------------------shader---------------------------
	virtual void UpdateShaderState();
	virtual void ResizeBuffer(UINT nWidth, UINT nHeight);
	//---------------------------------shader---------------------------

private:
	//begin func
	virtual bool CreateShaderValues();//texture, constbuffer etc.
	virtual bool CreateCS();
	//begin func

	//end func 
	virtual bool ReleaseShaderValues();
	//end func

	//set func
	virtual void SetShaderValues();
	//clean func
	virtual void CleanShaderValues();

	//bloom
	//2
	ID3D11Texture2D			 *m_pd3dtxtTempBloom{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvTempBloom{ nullptr };
	ID3D11UnorderedAccessView*m_pd3duavTempBloom{ nullptr };

private:

public:
	CCSHorizontalBlur(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CCSHorizontalBlur();
};

/*
첫단계에서는
그림이 다 그려진 srv를 입력으로 받아서
그것을 1024*16개의 픽셀을 1024개로 만든다.
*/