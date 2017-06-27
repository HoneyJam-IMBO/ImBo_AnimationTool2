#include "stdafx.h"
#include "TestCube.h"

bool CTestCube::Begin() {
	//object_id set
	//m_objectID = object_id::OBJECT_FBX_ELF;
	m_objectID = object_id::OBJECT_TEST;

	//Rotate(-90.f, 0.f, 0.f);
	//새로운 객체가 생겨나면 Begin에서 Component들을 set해준다. 
//	CComponent* pComponent = new CRotateComponent;
//	pComponent->Begin();
//	SetComponent(pComponent);

	CGameObject::Begin();
	return true;
}
bool CTestCube::End() {

	return CGameObject::End();
}
void CTestCube::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if (m_pTerrainContainer) m_xmf4x4World._42 = GetTerrainHeight() + 5.f;

}

CTestCube::CTestCube() : CGameObject("testcube", tag::TAG_DYNAMIC_OBJECT) {

}
CTestCube::~CTestCube() {

}