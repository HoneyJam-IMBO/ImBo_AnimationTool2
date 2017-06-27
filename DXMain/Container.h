#pragma once
#include "DXObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "Object.h"
#include "Camera.h"

//object, mesh, shader를 관리하는 녀석
//3가지를 가지고 그림 그리는 방식이 달라질 것으로 예상됨
//object들을 맴버 변수로 관리함 object의 가상화가 중요함
//mesh, shader도 마찮가지임

//컨테이너의 
//한가지 중요한 점은 mesh, shader, object를 관리한다는 것
class CContainer : public DXObject{
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState() {};
	virtual void CleanShaderState() {};

	virtual void UpdateShaderState() {};
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void RenderExcute() {};
	void Render();
	void Animate(float fTimeElapsed);

	void SetMesh(shared_ptr<CMesh> pMesh);
	void SetShader(shared_ptr<CShader> pShader);

	void SetObejcts(int n, CObject** ppObjects);
	void AddObject(CObject* pObject);
	void RemoveObject(CObject* pObject);
	list<CObject*>& GetObjectList() { return m_lpObjects; }

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

	//--------------------------관리 대상------------------------------
	//컨테이너의 객체들은 동적으로 변할 것이다. 그러므로 리스트로 관리하는 것이 맞다
	list<CObject*> m_lpObjects;

	shared_ptr<CMesh> m_pMesh{ nullptr };
	shared_ptr<CShader> m_pShader{ nullptr };
	shared_ptr<CCamera> m_pCamera{ nullptr };
	//--------------------------관리 대상------------------------------

public:
	CContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CContainer();

};