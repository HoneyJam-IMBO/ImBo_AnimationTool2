#include "stdafx.h"
#include "DebugRenderContainer.h"


//--------------------------container---------------------------------
void CDebugRenderContainer::UpdateShaderState(shared_ptr<CCamera> pCamera) {
	m_vpMesh[0]->UpdateShaderState();
	m_pShader->UpdateShaderState();
	for (auto p : m_vpTexture) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->UpdateShaderState();
	}

	//if (m_pGlobalBuffer) m_pGlobalBuffer->UpdateShaderState();
	//----------------------------update instance buffer--------------------------

	if (m_vpBuffer.empty()) return;

	int nInstance = 0;

	int nBuffer = 0;
	//map
	for (auto p : m_vpBuffer) {
		m_ppBufferData[nBuffer++] = p->Map();
	}
	for (auto pObject : m_lpObjects) {
			pObject->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
			nInstance++;
	}

	m_nInstance = nInstance;
	//unmap
	for (auto p : m_vpBuffer) {
		p->Unmap();
	}
	//----------------------------update instance buffer--------------------------


}
void CDebugRenderContainer::UpdateGlobalBuffer() {
	if (m_vpGlobalBuffer.empty()) return;

	int nBuffer = 0;
	//map
	for (auto p : m_vpGlobalBuffer) {
		m_ppGlobalBufferData[nBuffer++] = p->Map();
	}
	//set data
	//m_pGlobalObject->SetDebugGlobalBufferInfo(m_ppGlobalBufferData);
	m_pGlobalObject->SetGlobalBufferInfo(m_ppGlobalBufferData);
	//unmap
	for (auto p : m_vpGlobalBuffer) {
		p->Unmap();
	}

}

CDebugRenderContainer::CDebugRenderContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderContainer(pd3dDevice, pd3dDeviceContext) {
	
}
CDebugRenderContainer::~CDebugRenderContainer() {

}