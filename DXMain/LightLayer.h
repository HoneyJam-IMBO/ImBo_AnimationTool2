#pragma once
#include "Layer.h"
#include "Texture.h"

class CLightLayer : public CLayer {
public:
	//---------------------------dxobject---------------------------------
	bool Begin();
	virtual bool End();

	virtual void SetShaderState(shared_ptr<CCamera> pCamera);
	virtual void CleanShaderState(shared_ptr<CCamera> pCamera);

	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void RenderExcute(shared_ptr<CCamera> pCamera);
	//--------------------------container---------------------------------

private:
	virtual void CreateShaderState();

	//조명 계산을 위한 블랜드 state.
	ID3D11BlendState* m_pLightBlendState{ nullptr };
	ID3D11DepthStencilState* m_pLightDepthStencilState{ nullptr };
	ID3D11RasterizerState* m_pLightRasterizerState{ nullptr };

	//이전 상태 저장
	ID3D11BlendState* m_pPreBlendState{ nullptr };
	float* m_pPreBlendFactor{ nullptr };
	UINT m_PreSampleMask{ 0 };
	ID3D11DepthStencilState* m_pPreDepthStencilState{ nullptr };
	UINT m_PreStencilRef{ 0 };
	ID3D11RasterizerState* m_pPreRasterizerState{ nullptr };
public:
	CLightLayer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CLightLayer();

};