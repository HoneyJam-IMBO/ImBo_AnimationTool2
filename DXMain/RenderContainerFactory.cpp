#include "stdafx.h"
#include "RenderContainerFactory.h"

CRenderContainer* CRenderContainerFactory::CreateRenderContainer(int nMaxObjects) {

	//new
	CreateContainer();
	
	//create objects
	CreateMesh();
	CreateSharedBuffer(nMaxObjects);
	CreateShader();
	CreateTexture();
	CreateMaterial();
	//create objects
	
	//begin
	m_pContainer->Begin();
	
	return m_pContainer;
}

CRenderContainerFactory::CRenderContainerFactory(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject(pd3dDevice, pd3dDeviceContext) {
	m_pCamera = pCamera;
}
