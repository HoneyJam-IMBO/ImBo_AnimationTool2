#pragma once

#include "RenderContainer.h"

#include "Texture.h"
#include "PostProcessingShader.h"
#include "TestDeferredMesh.h"

#include "InstancingShader.h"
#include "DownScaleFirstPass.h"
#include "DownScaleSecondPass.h"

//bloom
#include "CSBloom.h"
//blur
#include "CSHorizontalBlur.h"
#include "CSVerticalBlur.h"

#define BLOOM_TEMP_TEX_NUM 2
struct stFinalPassCB {
	float fMiddleGrey;
	float fLumWhiteSqr;
	float fBloomScale;
	float pad;
};

class CPostProcessingRenderContainer : public CRenderContainer {

public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();


	virtual void CleanShaderState();

	virtual void UpdateShaderState();
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void RenderExcute();

	void SetFirstPassData(UINT width, UINT height);
	void SetAdaptation(float fAdaptation);//s
	void SetFinalPassData(float fMiddleGrey, float fWhite, float fBloomScale);
	void SetBloomThreshold(float fBloomThreshold);
	//--------------------------container---------------------------------

private:
	//final 풀스크린 드로우를 위한 cb
	ID3D11Buffer* m_pFinalPassCB{ nullptr };
	float m_fMeddleGrey{ 0.0 };
	float m_fWhite{ 0.0 };

	//bloom
	float m_fBloomScale{ 0.0 };

	//전체적인 cs라인에 유지해야될 데이터
	UINT m_nDownScaleGroup{ 0 };
	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };


	//휘도 down sacle을 위한 cs
	CDownScaleFirstPassShader* m_pCSDownScaleFirstPass{ nullptr };
	CDownScaleSecondPassShader* m_pCSDownScaleSecondPass{ nullptr };

	//bloom
	CCSBloom* m_pCSBloom{ nullptr };

	//blur 
	CCSHorizontalBlur* m_pCSHorizontalBlur{ nullptr };
	CCSVerticalBlur* m_pCSVerticalBlur{ nullptr };
	
public:
	CPostProcessingRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CPostProcessingRenderContainer();

};