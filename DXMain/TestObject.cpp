#include "stdafx.h"
#include "TestObject.h"


bool CTestObject::Begin() {
	//object_id set
	//m_objectID = object_id::OBJECT_END;
	//m_objectID = object_id::OBJECT_TEST;

	Rotate(-90.f, 0.f, 0.f);
	//새로운 객체가 생겨나면 Begin에서 Component들을 set해준다. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);

	CGameObject::Begin();
	return true;
}
bool CTestObject::End() {

	return CGameObject::End();
}
void CTestObject::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if( m_pTerrainContainer ) m_xmf4x4World._42 = GetTerrainHeight();
	
}
void CTestObject::Animate(float fTimeElapsed) {
	m_pRenderContainer = RCSELLER->GetRenderContainer(m_objectID);
	
	if (m_pAnimater){
		m_pAnimater->Update(fTimeElapsed);
		
		for (auto OBB : m_pAnimater->GetCurAnimationInfo()->GetActiveBoundingBox()) {
			//XMMATRIX xmmtxOffset = XMLoadFloat4x4(&m_pAnimater->GetMeshOffsetMtx());
			BoundingOrientedBox obb;
			OBB.Transform(obb, GetWorldMtx());
			DEBUGER->RegistOBB(obb);
		}
		for (auto coord : m_pAnimater->GetCurAnimationInfo()->GetSelectJoint()) {
			DEBUGER->RegistCoordinateSys(coord * GetWorldMtx());
		}
	}
}
CTestObject::CTestObject() : CGameObject("testobject", tag::TAG_DYNAMIC_OBJECT) {

}
CTestObject::~CTestObject() {

}