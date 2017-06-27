#include "stdafx.h"
#include "AdaptorRenderContainer.h"

//---------------------------dxobject---------------------------------
bool CAdaptorRenderContainer::Begin() {
	

	return true;
}
bool CAdaptorRenderContainer::End() {
	for (auto p = m_lpObjects.begin(); p != m_lpObjects.end(); ++p) {
		(*p)->End();
	}
	m_lpObjects.clear();

	if (m_pMesh) {
		m_pMesh->End();
	}
	if (m_pShader) {
		m_pShader->End();
	}
	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CAdaptorRenderContainer::UpdateShaderState() {
	m_pMesh->UpdateShaderState();
	m_pShader->UpdateShaderState();

	//----------------------------update instance buffer--------------------------
	int nInstance = 0;
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	m_pd3dDeviceContext->Map(m_pd3dInstancingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_VB_INSTANCE *pnInstances = (VS_VB_INSTANCE *)d3dMappedResource.pData;
	for (auto pObject = m_lpObjects.begin(); pObject != m_lpObjects.end(); ++pObject) {
		pnInstances[nInstance++].m_xmmtxWorld = XMMatrixTranspose((*pObject)->GetWorldMtx());
	}

	m_nInstance = nInstance;
	//for (int j = 0; j < nSphereObjects; j++)
	//{
	//	if (m_ppObjects[j])
	//	{
	//		if (m_ppObjects[j]->IsVisible(pCamera))
	//		{
	//			D3DXMatrixTranspose(&pnSphereInstances[nSphereInstances].m_d3dxTransform, &m_ppObjects[j]->m_d3dxmtxWorld);
	//			pnSphereInstances[nSphereInstances++].m_d3dxColor = RANDOM_COLOR;
	//		}
	//	}
	//}
	m_pd3dDeviceContext->Unmap(m_pd3dInstancingBuffer, 0);
	//----------------------------update instance buffer--------------------------

}
void CAdaptorRenderContainer::SetShaderState() {
	m_pMesh->SetShaderState();
	m_pShader->SetShaderState();
}
void CAdaptorRenderContainer::RenderExcute() {
	m_pMesh->RenderExcute(m_nInstance);
}

void CAdaptorRenderContainer::SetnullptrShaderState() {
	m_pMesh->SetnullptrShaderState();
	m_pShader->SetnullptrShaderState();
}

CAdaptorRenderContainer::CAdaptorRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderContainer(pCamera, pd3dDevice, pd3dDeviceContext) {

}
CAdaptorRenderContainer::~CAdaptorRenderContainer() {

}