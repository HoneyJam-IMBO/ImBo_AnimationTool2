#pragma once

#include "RenderContainer.h"
#include "AdaptorMesh.h"
#include "InstancingShader.h"


class CAdaptorRenderContainer : public CRenderContainer {
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void SetnullptrShaderState();

	virtual void UpdateShaderState();
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void RenderExcute();
	//--------------------------container---------------------------------


public:
	CAdaptorRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CAdaptorRenderContainer();

};