#pragma once
#include "DXObject.h"
#include "GameObject.h"

//dxobject
#include "Mesh.h"
#include "RenderShader.h"
#include "Texture.h"
#include "Buffer.h"
#include "GlobalBuffer.h"
#include "Material.h"
#include "Animater.h"//animater!!
//dxobject

#include "Camera.h"



class CRenderContainer : public DXObject {
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void UpdateGlobalBuffer();
private:
	virtual void RenderExcute();
	void RenderExcuteWithOutObject();
public:
	void Render(shared_ptr<CCamera> pCamera);
	void RenderWithOutObject(shared_ptr<CCamera> pCamera);

	shared_ptr<CMesh> GetMesh(UINT index = 0) { return m_vpMesh[index]; }
	vector<shared_ptr<CMesh>>& GetvMesh() { return m_vpMesh; }
	//set이 아니라이제 add다.
	void ClearMesh();
	void ClearAnimater();
	void AddMesh(shared_ptr<CMesh> pMesh);
	void SetMesh(shared_ptr<CMesh> pMesh);
	void SetShader(shared_ptr<CRenderShader> pShader);
	
	//void AddMesh(shared_ptr<CMesh> pMesh);
	//void AddShader(shared_ptr<CRenderShader> pShader);
	void AddTexture(shared_ptr<CTexture> pTexture);
	void AddBuffer(shared_ptr<CBuffer> pBuffers);
	void AddInstanceBuffer(shared_ptr<CBuffer> pBuffers);
	void AddMaterial(shared_ptr<CMaterial> pMaterial);
	//global buffer
	void AddGlobalBuffer(shared_ptr<CGlobalBuffer> pGlobalBuffer);
	void SetGlobalObject(CGlobalObject* pGlobalObject) { m_pGlobalObject = pGlobalObject; }
	//global buffer
	//animater!
	void SetAnimater(shared_ptr<CAnimater> pAnimater) { m_pAnimater = pAnimater; }
	shared_ptr<CAnimater> GetAnimater() { return m_pAnimater; }
	//animater!

	void SetObejcts(int n, CGameObject** ppObjects);
	void AddObject(CGameObject* pObject);
	void RemoveObject(CGameObject* pObject);
	void ClearObjectList() { m_lpObjects.clear(); }//그림 다 그리고 clear
	list<CGameObject*>& GetObjectList() { return m_lpObjects; }

	void ClearTextures() { m_vpTexture.clear(); }
	//--------------------------container---------------------------------
protected:
	//--------------------------관리 대상------------------------------
	//컨테이너의 객체들은 동적으로 변할 것이다. 그러므로 리스트로 관리하는 것이 맞다
	int m_nInstance{ 0 };
	list<CGameObject*> m_lpObjects;

	vector<shared_ptr<CMesh>> m_vpMesh;
	shared_ptr<CRenderShader> m_pShader;
	vector<shared_ptr<CTexture>> m_vpTexture;
	int m_nTexture{ 0 };
	vector<shared_ptr<CBuffer>> m_vpBuffer;
	shared_ptr<CAnimater> m_pAnimater{ nullptr };//animater

	int m_nBuffer{ 0 };
	//global buffer
	vector<shared_ptr<CGlobalBuffer>> m_vpGlobalBuffer;
	int m_nGlobalBuffer{ 0 };
	CGlobalObject* m_pGlobalObject{ nullptr };
	//global buffer
	vector<shared_ptr<CMaterial>> m_vpMaterial;
	int m_nMaterial{ 0 };
	//--------------------------관리 대상------------------------------

	void** m_ppBufferData{ nullptr };
	void** m_ppGlobalBufferData{ nullptr };
public:
	CRenderContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CRenderContainer();

};

typedef map<object_id, CRenderContainer*> mapRenderContainer;
typedef pair<object_id, CRenderContainer*> pairRenderContainer;


/*
컨테이너 - AnimateContainer, RenderContainer로 분리
{
렌더 과정 - 
	1. AnimateContainer로 모든 object Animate
	2. 그림 그릴 object RenderContainer로 AddObject
	3. RenderContainer에서 카메라 프러스텀 컬링 후 인스턴싱으로 렌더
여기에 공간까지 들어간다?
CScene은 Space를 관리한다.
Spcae는 Container를 관리하는데 AnimateContainer다.
Animate할 때 중요한게 공간을 벗어난 경우 바로 다른 공간에 이식하는 것이 아니라
따로 빼놓았다가 넣어 줘야 한다. pExcludeContainer
여기서 ExcluderContainer는 객체마다 새로 만들어야 할 꺼 같은데

----------------------------------------------------
AnimateContainer/ object_id{enum} 필요성 대두

enum으로 0-nObject 객체 종류별로 enum해서 이걸 objectid 값으로 가지고 있어서
빠르게 분류해 낼 수 있다면 AnimateContainer는 object가 추상화가 잘 되있다는 가정하에
객체별로 없어도 되고 하나만 공간마다 하나씩만 있으면 된다.
----------------------------------------------------

----------------------------------------------------
RenderContainer/ object별로 필요하다
RenderContainer는 instancing해야 하니까 정말 각 객체마다 존재해야 한다.
따라서 RenderContainer는 CScene이 관리하고
Scene에 RenderContainer는 map으로 관리한다. map<object_id, RenderContainer> 

AnimateContainer에서 enum이로 분류를 빠르게 해서 RenderContainer에게 넘겨주면
그걸 받아서 처리하도록 한다.

RenderContainer는 CScene이 관리하는것
}
*/