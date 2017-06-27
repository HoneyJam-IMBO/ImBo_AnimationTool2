#pragma once

#include "InstancingObjectRenderContainer.h"
#include "PlaneMesh.h"
#include "TestShader.h"
#include "InstancingShader.h"


class CPlaneRenderContainer : public CInstancingObjectRenderContainer {
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
	//--------------------------container---------------------------------


public:
	CPlaneRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CPlaneRenderContainer();

};