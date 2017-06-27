#pragma once
#include "RenderContainerFactory.h"
#include "CoreMesh.h"
#include "InstancingShader.h"
#include "CoreRenderContainer.h"

//이 펙토리는 product를 return하는 컨테이너다.
class CCoreRenderContainerFactory : public CRenderContainerFactory {

private:
	virtual void CreateContainer();
	virtual void CreateMesh(int nMaxObject);
	virtual void CreateShader();
	virtual void CreateTexture();
	virtual void CreateMaterial();

public:
	CCoreRenderContainerFactory(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderContainerFactory(pCamera, pd3dDevice, pd3dDeviceContext) {}
	virtual ~CCoreRenderContainerFactory() {}
};
