#pragma once

#include "RenderContainer.h"
#include "InstancingObjectRenderContainer.h"
#include "CoreMesh.h"
#include "BoostMesh.h"
#include "WeaponMesh.h"
#include "AdaptorMesh.h"

#include "Part.h"
#include "Core.h"

#include "InstancingShader.h"
#include "TestShader.h"
#include "TestMesh.h"

//typedef map<part_id, CRenderContainer*> mapPartRenderContainer;
//typedef pair<part_id, CRenderContainer*> pairPartRenderContainer;

class CCoreRenderContainer : public CRenderContainer {
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

	void CreateCoreContainer(int nMaxObjects);
	void CreateWeaponContainer(int nMaxObjects);
	void CreateBoostContainer(int nMaxObjects);
	void CreateAdaptorContainer(int nMaxObjects);

	//--------------------------container---------------------------------

private:
	//mapPartRenderContainer m_mPartRenderContainer;

public:
	CCoreRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CCoreRenderContainer();

};