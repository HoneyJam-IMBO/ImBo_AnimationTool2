#pragma once

#include "RenderContainer.h"

class CDebugRenderContainer : public CRenderContainer{
public:

	//---------------------------dxobject---------------------------------
	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void UpdateGlobalBuffer();


public:
	CDebugRenderContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CDebugRenderContainer();

};

typedef map<object_id, CRenderContainer*> mapRenderContainer;
typedef pair<object_id, CRenderContainer*> pairRenderContainer;
