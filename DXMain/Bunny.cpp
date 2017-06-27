#include "stdafx.h"
#include "Bunny.h"

//void TW_CALL AnimationSelectCallback(void * clientData) {
//	CAnimationInfo* pAnimInfo = reinterpret_cast<CAnimationInfo*>(clientData);
//	pAnimInfo->SelectAnimationProc();
//}
//void TW_CALL JointSelectButtonCallback(void * clientData) {
//	CBoundingBox* pOBB = reinterpret_cast<CBoundingBox*>(clientData);
//	if(pOBB->GetActive())
//		pOBB->SetActive(false);
//	else
//		pOBB->SetActive(true);
//	
//}
bool CBunny::Begin() {
	//object_id set
	m_objectID = object_id::OBJECT_FBX_BUNNY;
	//m_objectID = object_id::OBJECT_TEST;

	CGameObject::Begin();
//	Rotate(-90.f, 0.f, 0.f);
	//새로운 객체가 생겨나면 Begin에서 Component들을 set해준다. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);


	return true;
}
bool CBunny::End() {

	return CGameObject::End();
}
void CBunny::Animate(float fTimeElapsed){
	if (m_pAnimater)m_pAnimater->Update(fTimeElapsed);
	//for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
	//	m_pRenderContainer->GetMesh(i).get()->Update(fTimeElapsed);
	//}

}
void CBunny::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if (m_pTerrainContainer) m_xmf4x4World._42 = GetTerrainHeight();
}

CBunny::CBunny() : CGameObject("testobject", tag::TAG_DYNAMIC_OBJECT) {

}
CBunny::~CBunny() {

}