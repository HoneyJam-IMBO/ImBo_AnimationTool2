#pragma once
#include "ComputeShader.h"

struct stFirstPassCB {
	UINT nWidth;
	UINT nHeight; //res
	UINT nTotalPixels;//domain
	UINT nGroupSize;//groupsize

	float fAdaptation;//적응 값
	float fBloomThreshold;//bloom 임계값
	XMFLOAT2 pad;
};

class CDownScaleFirstPassShader : public CComputeShader {
public:
	//---------------------------------shader---------------------------
	virtual void UpdateShaderState();
	virtual void ResizeBuffer(UINT nWidth, UINT nHeight);
	//---------------------------------shader---------------------------

	void SetFirstPassData(UINT width, UINT height, UINT nDownScaleGroups);
	void SetAdaptation(float fAdaptation);
	void SetBloomThreshold(float fBloomThreshold);
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

private:
	//cs data
	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };
	UINT m_nDownScaleGroup{ 0 };
	float m_fAdaptation{ 0.0 };
	//cs를 위한 shader resource
	ID3D11Buffer* m_pFirstPassBuffer{ nullptr };
	ID3D11UnorderedAccessView* m_pFirstPassUAV{ nullptr };
	ID3D11ShaderResourceView* m_pFirstPassSRV{ nullptr };
	ID3D11Buffer* m_pFirstPassCB{ nullptr };


	//bloom
	float m_fBloomThreshold{ 0.0f };
	//1
	ID3D11Texture2D			 *m_pd3dtxtDownCasting{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvDownCasting{ nullptr };
	ID3D11UnorderedAccessView*m_pd3duavDownCasting{ nullptr };

public:
	CDownScaleFirstPassShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CDownScaleFirstPassShader();
};

/*
첫단계에서는 
그림이 다 그려진 srv를 입력으로 받아서
그것을 1024*16개의 픽셀을 1024개로 만든다.
*/