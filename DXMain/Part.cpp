#include "stdafx.h"
#include "Part.h"
#include "RenderContainer.h"

bool CPart::Begin() { 
	XMStoreFloat4x4(&m_xmf4x4Local, XMMatrixIdentity());

	return CGameObject::Begin();
};
bool CPart::End() { 

	return CGameObject::End();
};
bool CPart::Equip(int slot, CPart* pPart) {
	//1. set Equip
	if (false == SetEquip(slot, pPart)) return false;
	//2. inform change state to core
	ChangePartState(slot, part_state::STATE_NEW, pPart);

	return true;
}

bool CPart::Equiped(int slot, CPart* pAdaptor) {
	//1. set Parent
	if (false == SetParent(slot, pAdaptor)) return false;
	//2. set my localposition
	if (false == SetLocalLocation(slot, pAdaptor)) return false;

	return true; 
}

//void CPart::RegistToContainer(mapRenderContainer & mRenderContainer){
//	mRenderContainer[m_objectID]->AddObject(this);
//}
