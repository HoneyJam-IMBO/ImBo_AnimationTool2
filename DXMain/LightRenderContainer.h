#pragma once

#include "RenderContainer.h"

#include "DirectionalLightMesh.h"
#include "PointLightMesh.h"
#include "SpotLightMesh.h"
#include "CapsuleLightMesh.h"

#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CapsuleLight.h"

#include "ConstantBuffer.h"

#include "DirectionalLightShader.h"
#include "PointLightShader.h"
#include "SpotLightShader.h"
#include "CapsuleLightShader.h"

typedef map<light_id, CRenderContainer*> mapLightRenderContainer;
typedef pair<light_id, CRenderContainer*> pairLightRenderContainer;

class CLightRenderContainer : public CRenderContainer {
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

	void CreateDirectionalContainer(int nMaxObjects);
	void CreatePointContainer(int nMaxObjects);
	void CreateSpotContainer(int nMaxObjects);
	void CreateCapsuleContainer(int nMaxObjects);

	//--------------------------container---------------------------------

private:
	void CreateShaderState();


	mapLightRenderContainer m_mLightRenderContainer;

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
	CLightRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CLightRenderContainer();

};