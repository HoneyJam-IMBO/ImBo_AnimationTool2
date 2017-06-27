#include "stdafx.h"
#include "AdaptorPart.h"
#include "RenderContainer.h"

//----------------------------dxobject-----------------------------
bool CAdaptorPart::Begin() {

	//m_partID = part_id::PART_ADAPTOR;
	m_objectID = object_id::OBJECT_ADAPTOR;

	//hp set
	m_fHP = 100;
	m_fSize = 10;

	//slot Init
	m_ppPart = new CPart*[6];
	for (int i = 0; i < 6; ++i) {
		m_ppPart[i] = nullptr;
	}
	//parent slot Init
	m_nParentSlot = 0;

	//return CPart::Begin();
	//새로운 객체가 생겨나면 Begin에서 Component들을 set해준다. 

	CComponent* pComponent = new CRotateComponent;
	pComponent->Begin();
	SetComponent(pComponent);

	return CPart::Begin();
}
bool CAdaptorPart::End() {
	for (int i = 0; i < 6; ++i) {
		if (m_ppPart[i]) delete m_ppPart[i];
	}
	if(m_ppPart) delete[] m_ppPart;

	return CPart::End();
}
//----------------------------dxobject-----------------------------

////-----------------------------parts-------------------------------
////core의 state변경 전달 - input, ai etc..
//void CCore::ChangeCoreState() {
//	for (int i = 0; i < 6; ++i) {
//		if (m_ppPart[i])m_ppPart[i]->ChangeCoreState();
//	}
//};
////part의 state벼경 전달 - delete, new ..
//void CCore::ChangePartState() {
//
//};
////조립 시 adaptor의 위치에 따라 자신의 local좌표 변경
//void CCore::SetLocalPosition(int slot, CPart* pAdaptor) {
//
//};
//부품슬롯 6개<rul -r-u-l순으로 0-5번>/ 
//adaptor는 core와 같은 순의 슬롯을 가진다. 
//boost, weapon은 조립한 후 위치조정의 필요성이 있지만 adaptor는 아니다. 따라서 회전시키지 않는다.
bool CAdaptorPart::SetEquip(int slot, CPart* pPart) {

	if (nullptr == m_ppPart[slot]) {
		//장비 set 
		if (false == pPart->Equiped(slot, this))return false;
		m_ppPart[slot] = pPart;

		return true;
	}

	return false;

}
void CAdaptorPart::GetPartsList(list<CPart*>& lParts) {

	lParts.push_back(this);
	for (int i = 0; i < 6; ++i) {
		if (i == m_nParentSlot) continue;
		//각 slot에 parts가 있다면 list를 얻으러 가자!
		if (m_ppPart[i]) 
			m_ppPart[i]->GetPartsList(lParts);
	}

}
////-----------------------------parts-------------------------------
void CAdaptorPart::Animate(float fTimeElapsed, XMMATRIX& xmmtxParents) {

	//adaptor의 Animate!
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4Local), xmmtxParents));

	//part의 Animate! 는 조금 다르다 .계층 구조 Animate 를 진행
	XMMATRIX xmmtxParent = XMLoadFloat4x4(&m_xmf4x4World);
	for (int i = 0; i < 6; ++i) {
		if (i == m_nParentSlot) continue;
		//각 slot에 parts가 있다면 Animate!
		if (m_ppPart[i]) m_ppPart[i]->Animate(fTimeElapsed, xmmtxParent);
	}

}

void CAdaptorPart::ChangePartState(int slot, part_state partState, CPart* pPart) {

	m_pParent->ChangePartState(slot, partState, pPart);

}


//parent
bool CAdaptorPart::SetParent(int slot, CPart* pParent) {
	if (nullptr == pParent) return false;

	m_nParentSlot = (slot + 3) % 6;
	m_ppPart[m_nParentSlot] = pParent;
	m_pParent = pParent;

	return true;
}
bool CAdaptorPart::SetLocalLocation(int slot, CPart* pParent) {
	//방향 제작 look
	XMVECTOR xmvDir;

	switch (slot) {
	case 0:
		//right
		xmvDir = XMVectorSet(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13, 0.f);
		break;
	case 1:
		//up
		xmvDir = XMVectorSet(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23, 0.f);
		break;
	case 2:
		//look
		xmvDir = XMVectorSet(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33, 0.f);
		break;
	case 3:
		//-right
		xmvDir = -XMVectorSet(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13, 0.f);
		break;
	case 4:
		//-up
		xmvDir = -XMVectorSet(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23, 0.f);
		break;
	case 5:
		//-look
		xmvDir = -XMVectorSet(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33, 0.f);
		break;
	default:
		return false;
		break;
	}

	//function part는 기능이 발현되는 vector가 look vector이기 때문에 회전 및 이동을 해주어야 한다.
	xmvDir = XMVector3Normalize(xmvDir);
	//이동 벡터 제작
	xmvDir = xmvDir*m_pParent->GetSize();

	XMFLOAT3 xmf3Dir;
	XMStoreFloat3(&xmf3Dir, xmvDir);
	m_xmf4x4Local._41 += xmf3Dir.x;
	m_xmf4x4Local._42 += xmf3Dir.y;
	m_xmf4x4Local._43 += xmf3Dir.z;

	return true;
}

void CAdaptorPart::RegistToContainer(){
	m_pRenderContainer->AddObject(this);

	for (int i = 0; i < 6; ++i) {
		if (i == m_nParentSlot) continue;
		//각 slot에 parts가 있다면 Animate!
		if (m_ppPart[i]) m_ppPart[i]->RegistToContainer();
	}
}

CAdaptorPart::CAdaptorPart() : CPart() {

};
CAdaptorPart::~CAdaptorPart() {

};