#include "stdafx.h"
#include "AnimateContainer.h"

//---------------------------dxobject---------------------------------
bool CAnimateContainer::Begin(CRenderContainerSeller* pSeller) {

	return true;
}
bool CAnimateContainer::End() {
	for (auto p = m_lpObjects.begin(); p != m_lpObjects.end(); ++p) {
		(*p)->End();
	}
	m_lpObjects.clear();
	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------

void CAnimateContainer::Animate(float fTimeElapsed, CAnimateContainer* pExcludeContainer) {
	//모든 객체 Animate
	for (auto p = m_lpObjects.begin(); p != m_lpObjects.end(); ++p) {
		(*p)->Animate(fTimeElapsed);
	}
}

//--------------------------container 불변 함수---------------------------------
void CAnimateContainer::SetObejcts(int n, CGameObject** ppObjects) {
	if (!ppObjects) return;

	for (int i = 0; i < n; ++i) {
		m_lpObjects.emplace_back(ppObjects[i]);
	}
}

void CAnimateContainer::AddObject(CGameObject* pObject) {
	if (!pObject) return;

	m_lpObjects.emplace_back(pObject);
}
void CAnimateContainer::RemoveObject(CGameObject* pObject) {
	if (!pObject) return;
	//if (0 == m_lpObjects.size()) return;

	m_lpObjects.remove_if([&pObject](CGameObject* pO) {
		return pObject == pO;
	});
}

//--------------------------container 불변 함수---------------------------------

CAnimateContainer::CAnimateContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("animatecontainer", pd3dDevice, pd3dDeviceContext) {
	
}
CAnimateContainer::~CAnimateContainer() {

}