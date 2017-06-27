#pragma once

#include "Component.h"
#include "Camera.h"
#include "Object.h"

class CTerrainContainer;
class CAnimater;

//rendercontainer 더미 클래스
class CRenderContainer;
//class CRenderContainerSeller;

typedef std::pair<component_id, CComponent*> pairComponent;
typedef std::map<component_id, CComponent*> mapComponent;


struct VS_VB_INSTANCE {
	XMMATRIX m_xmmtxWorld;
};
class CMesh;
struct StructLoadTextureFile {
	shared_ptr<CMesh> m_pMesh;
	string m_sName;
	int m_indexSelectTexture{ 0 };
};

class CGameObject : public CObject {
public:
	//----------------------------object-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	object_id GetObjectID() { return m_objectID; }
	void SetObjectID(object_id id) { m_objectID = id; }

	void SetActive(bool b) { m_bActive = b; }
	bool& GetActive() { return m_bActive; }

	XMVECTOR GetRight();
	XMVECTOR GetUp();
	XMVECTOR GetLook();
	void SetRight(XMVECTOR xmvRight);
	void SetUp(XMVECTOR xmvUp);
	void SetLook(XMVECTOR xmvLook);

	void SetWorldMtx(XMMATRIX mtxWorld);
	virtual XMMATRIX GetWorldMtx();
	void SetMeshOffsetMtx(XMMATRIX mtxMeshOffset) { XMStoreFloat4x4(&m_xmf4x4MeshOffset, mtxMeshOffset); }
	XMMATRIX GetMeshOffsetMtx() { return XMLoadFloat4x4(&m_xmf4x4MeshOffset); }
	virtual void SetPosition(XMVECTOR pos);
	void SetPositionX(const float pos);
	void SetPositionY(const float pos);
	void SetPositionZ(const float pos);
	float GetPositionX() { return m_xmf4x4World._41; };
	float GetPositionY() { return m_xmf4x4World._42; };
	float GetPositionZ() { return m_xmf4x4World._43; };
	XMVECTOR GetPosition();
	XMVECTOR GetQuaternion() { return XMLoadFloat4(&m_xmf4Quaternion); }
	void SetQuaternion(XMVECTOR xmv) { XMStoreFloat4(&m_xmf4Quaternion, xmv); }
	XMVECTOR GetScale() { return XMVectorSet(m_xmf4Scale.x*m_xmf4Scale.w, m_xmf4Scale.y*m_xmf4Scale.w, m_xmf4Scale.z*m_xmf4Scale.w, 1.0f); }
	void SetScale(XMVECTOR xmv);

	void SetRotation(XMMATRIX mtxRotation);

	//----------------------------component------------------------
	//test animate func
	virtual void Move(XMVECTOR xmvDir, float fDistance);
	virtual void Rotate(XMMATRIX xmMtx);
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void RotateWorldAxis(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	float& GetRotateX() { return m_xmf3Rotate.x; };
	float& GetRotateY() { return m_xmf3Rotate.y; };
	float& GetRotateZ() { return m_xmf3Rotate.z; };

	//자신의 component 실행 + a
	virtual void Animate(float fTimeElapsed);
	bool SetComponent(CComponent* pComponenet);
	CComponent* GetComponenet(const component_id& componenetID);
	const CComponent* GetComponenet(const component_id& componenetID)const;
	void ClearComponents();
	//----------------------------component------------------------

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);
	
	//render container controll
	virtual void RegistToContainer();
	CRenderContainer* GetRenderContainer() { return m_pRenderContainer; }

	//debuger container controll
	virtual void RegistToDebuger();

	//space
	void SetSpaceIndex(int index) { m_spaceIndex = index; }
	int GetSpaceIndex() { return m_spaceIndex;	}

	//terrain 
	float GetTerrainHeight();
	void SetTerrainContainer(CTerrainContainer* pTerrainContainer) { m_pTerrainContainer = pTerrainContainer; };

	//aabb
	virtual bool IsVisible(shared_ptr<CCamera> pCamera);//계층구조의 녀석들은 다시 만들어줄 필요가 있음
	void GetMainBoundingBox(BoundingBox& out);
	//ray picking 
	bool CheckPickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distance);
	virtual void PickingProc();
	void CreateObjectUI();
	void CreateMeshUI();
	void CreateMenuMeshTextureUI();

	//mesh offset mtx
	XMFLOAT4X4 m_xmf4x4MeshOffset;
	//world
	XMFLOAT4X4 m_xmf4x4World;
	XMFLOAT3 m_xmf3Position{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Quaternion;
	XMFLOAT4 m_xmf4Scale{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT3 m_xmf3Rotate{ 0.f,0.f, 0.f };

	//texture select ui를 위한 함수
	int& GetSelectMeshIndex() { return m_indexSelectMesh; }
	void SetSelectMeshIndex(float& index) { 
		m_indexSelectMesh = index; 
	}

	shared_ptr<CAnimater> GetAnimater() { return m_pAnimater; }
	void SetMeshMode(int mode);
	int GetMeshMode() { return m_indexSelectTexture; }
protected:
	int m_indexSelectMesh{ 0 };
	int m_indexSelectTexture{ 0 };

	BoundingBox m_OriBoundingBox;

	bool m_bIsVisible{ true }; 
	bool m_bActive{ true };

	//terrain container
	CTerrainContainer* m_pTerrainContainer{ nullptr };

	//component
	mapComponent m_mapComponents;

	CRenderContainer* m_pRenderContainer{ nullptr };
	//object_id
	object_id m_objectID{ object_id::OBJECT_END };

	//space
	int m_spaceIndex{ 0 };
	//animater
	shared_ptr<CAnimater> m_pAnimater{ nullptr };

	//texture ui를 위한 변수
	vector<StructLoadTextureFile> m_vStructLoadTextureFile;
public:
	CGameObject(string name, tag t = tag::TAG_DEFAULT);
	virtual ~CGameObject();
};