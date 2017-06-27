#pragma once
#include "DXObject.h"
#include "RenderContainer.h"
#include "Object.h"
#include "Camera.h"


//이 펙토리는 product를 return하는 컨테이너다.
class CRenderContainerFactory : public DXObject {
public:
	//이거만 불림
	CRenderContainer* CreateRenderContainer(int nMaxObjects);
protected:
	shared_ptr<CCamera> m_pCamera{ nullptr };
	CRenderContainer* m_pContainer{ nullptr };

private:
	//비어있는 RenderContainer를 넣으면 동적 할당해서 값을 채워 나간다.
	
	virtual void CreateContainer() {};
	virtual void CreateMesh() {};
	virtual void CreateSharedBuffer(int nMaxObjects) {};
	virtual void CreateShader() {};
	virtual void CreateTexture() {};
	virtual void CreateMaterial() {};
	virtual void BeginRenderContainer() {};

public:
	CRenderContainerFactory(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CRenderContainerFactory() {};
};
