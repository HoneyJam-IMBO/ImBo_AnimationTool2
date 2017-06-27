#pragma once
#include "Layer.h"
#include "SkyBox.h"

//class CRenderContainerSeller;

class CObjectLayer : public CLayer {
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

	ID3D11DepthStencilState* m_pd3dDepthStencilState{ nullptr };
	ID3D11DepthStencilState* m_pd3dTempDepthStencilState{ nullptr };
	UINT m_TempStencil{ 0 };

	ID3D11RasterizerState* m_pd3dSpaceRSState{ nullptr };
	ID3D11RasterizerState* m_pd3dTempRSState{ nullptr };

public:
	CObjectLayer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CObjectLayer();

};