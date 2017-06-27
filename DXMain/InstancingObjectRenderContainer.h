#pragma once
#include "RenderContainer.h"
#include "Object.h"
#include "InstanceBuffer.h"

class CInstancingObjectRenderContainer : public CRenderContainer {
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
private:
	virtual void RenderExcute();

	virtual void CreateSharedBuffer(int nMaxObjects, UINT nStrides);//instance buffer 제작부분 항상 setmesh이후에 해야한다.

	ID3D11Buffer* CreateInstancingBuffer(int nObject, UINT nStrides, void* pData);
	//--------------------------container---------------------------------
protected:
	//--------------------------instance-------------------------------
	int m_nMaxObjects{ 0 };
	UINT m_nInstancingBufferStrides{ 0 };
	UINT m_nInstancingBufferOffsets{ 0 };
	ID3D11Buffer* m_pd3dInstancingBuffer{ nullptr };
	UINT m_nInstance{ 1 };
	//--------------------------instance-------------------------------

public:
	CInstancingObjectRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CInstancingObjectRenderContainer();

};
