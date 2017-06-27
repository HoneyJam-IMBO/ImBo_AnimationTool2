#include "stdafx.h"
#include "InstancingObjectRenderContainer.h"

//---------------------------dxobject---------------------------------
bool CInstancingObjectRenderContainer ::Begin() {

	return true;
}
bool CInstancingObjectRenderContainer ::End() {
	
	if (m_pd3dInstancingBuffer) m_pd3dInstancingBuffer->Release();

	return CRenderContainer::End();
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CInstancingObjectRenderContainer ::UpdateShaderState() {
	m_pMesh->UpdateShaderState();
	m_pShader->UpdateShaderState();
	if (m_pTexture) m_pTexture->UpdateShaderState();
	if (m_pMaterial) m_pMaterial->UpdateShaderState();

	//----------------------------update instance buffer--------------------------
	
	if (!m_pBuffer) return;

	int nInstance = 0;
	//D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	//m_pd3dDeviceContext->Map(m_pd3dInstancingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	void** ppData = m_pBuffer->Map();
	for (auto pObject : m_lpObjects) {
		pObject->SetInstanceBufferInfo(ppData, nInstance, m_pCamera);
		nInstance++;
	}

	m_nInstance = nInstance;
	/*for (int j = 0; j < nSphereObjects; j++)//camera culling code
	{
	if (m_ppObjects[j])
	{
	if (m_ppObjects[j]->IsVisible(pCamera))
	{
	D3DXMatrixTranspose(&pnSphereInstances[nSphereInstances].m_d3dxTransform, &m_ppObjects[j]->m_d3dxmtxWorld);
	pnSphereInstances[nSphereInstances++].m_d3dxColor = RANDOM_COLOR;
	}
	}
	}*/
	//m_pd3dDeviceContext->Unmap(m_pd3dInstancingBuffer, 0);
	m_pBuffer->Unmap();
	//----------------------------update instance buffer--------------------------

}
void CInstancingObjectRenderContainer ::SetShaderState() {
	m_pMesh->SetShaderState();
	m_pShader->SetShaderState();
	if (m_pTexture) m_pTexture->SetShaderState();
	if (m_pMaterial) m_pMaterial->SetShaderState();
	if (m_pBuffer) m_pBuffer->SetShaderState();
}
void CInstancingObjectRenderContainer ::RenderExcute() {
	m_pMesh->RenderExcute(m_nInstance);
}
void CInstancingObjectRenderContainer ::CleanShaderState() {
	m_pMesh->CleanShaderState();
	m_pShader->CleanShaderState();
	if (m_pTexture) m_pTexture->CleanShaderState();
	if (m_pMaterial) m_pMaterial->CleanShaderState();
	if (m_pBuffer) m_pBuffer->CleanShaderState();
}

void CInstancingObjectRenderContainer ::CreateSharedBuffer(int nMaxObjects, UINT nStrides) {

	//------------------------------create instance buffer--------------------
	m_nInstancingBufferStrides = nStrides;
	m_nInstancingBufferOffsets = 0;
	m_pd3dInstancingBuffer = CreateInstancingBuffer(nMaxObjects, m_nInstancingBufferStrides, nullptr);
	m_pMesh->AssembleToVertexBuffer(1, &m_pd3dInstancingBuffer, &m_nInstancingBufferStrides, &m_nInstancingBufferOffsets);

}

ID3D11Buffer* CInstancingObjectRenderContainer ::CreateInstancingBuffer(int nObjects, UINT nBufferStride, void *pBufferData)
{
	m_nMaxObjects = nObjects;

	ID3D11Buffer *pd3dInstanceBuffer = NULL;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	/*버퍼의 초기화 데이터가 없으면 동적 버퍼로 생성한다. 즉, 나중에 매핑을 하여 내용을 채우거나 변경한다.*/
	d3dBufferDesc.Usage = (pBufferData) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nBufferStride * nObjects;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = (pBufferData) ? 0 : D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, (pBufferData) ? &d3dBufferData : NULL, &pd3dInstanceBuffer);
	return(pd3dInstanceBuffer);
}
//--------------------------container 불변 함수---------------------------------

CInstancingObjectRenderContainer::CInstancingObjectRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderContainer(pCamera, pd3dDevice, pd3dDeviceContext) {
	m_pCamera = pCamera;
}
CInstancingObjectRenderContainer::~CInstancingObjectRenderContainer () {

}