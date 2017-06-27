#include "stdafx.h"
#include "PlaneRenderContainer.h"

//---------------------------dxobject---------------------------------
bool CPlaneRenderContainer::Begin() {
	
	//--------------------------------메쉬 제작------------------------
	shared_ptr<CMesh> pMesh = make_shared<CPlaneMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	//--------------------------------메쉬 제작------------------------
	//--------------------------------쉐이더 제작-----------------------
	shared_ptr<CRenderShader> pShader = make_shared<CTestShader>(m_pd3dDevice, m_pd3dDeviceContext);
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//model
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//color
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//uv
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	pShader->CreateVS(TEXT("VertexShader.cso"), vertexDesc, 7);
	pShader->CreatePS(TEXT("PixelShader.cso"));
	pShader->Begin();
	SetShader(pShader);
	//--------------------------------쉐이더 제작-----------------------
	//-------------------------------재질 제작-----------------------------
	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	pMaterial->Begin(XMFLOAT4(0.5, 0.5, 0.0, 1.0), 8, 2);
	SetMaterial(pMaterial);
	//-------------------------------재질 제작-----------------------------	

	//-------------------------------버퍼 제작-----------------------------	
	shared_ptr<CInstanceBuffer> pConstantBuffer = make_shared<CInstanceBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	UINT pBufferStrides[1] = { sizeof(VS_VB_INSTANCE) };
	UINT pObjects[1] = { 1 };
	UINT pOffsets[1] = { 0 };
	pConstantBuffer->Begin(1, pObjects, pBufferStrides, pOffsets);
	SetInstanceBuffer(pConstantBuffer);
	//-------------------------------버퍼 제작-----------------------------	

	return true;
}
bool CPlaneRenderContainer::End() {
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
void CPlaneRenderContainer::UpdateShaderState() {
	m_pMesh->UpdateShaderState();
	m_pShader->UpdateShaderState();

	//----------------------------update instance buffer--------------------------
	int nInstance = 0;
	//D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	//m_pd3dDeviceContext->Map(m_pd3dInstancingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	//VS_VB_INSTANCE *pnInstances = (VS_VB_INSTANCE *)d3dMappedResource.pData;
	void** ppData = m_pBuffer->Map();
	for (auto pObject : m_lpObjects) {
		pObject->SetInstanceBufferInfo(ppData, nInstance, m_pCamera);
		nInstance++;
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
	//m_pd3dDeviceContext->Unmap(m_pd3dInstancingBuffer, 0);
	m_pBuffer->Unmap();
	//----------------------------update instance buffer--------------------------

}
void CPlaneRenderContainer::SetShaderState() {
	m_pMesh->SetShaderState();
	m_pShader->SetShaderState();
	m_pMaterial->SetShaderState();
}
void CPlaneRenderContainer::RenderExcute() {
	m_pMesh->RenderExcute(m_nInstance);
}

void CPlaneRenderContainer::CleanShaderState() {
	m_pMesh->CleanShaderState();
	m_pShader->CleanShaderState();
}

CPlaneRenderContainer::CPlaneRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CInstancingObjectRenderContainer(pCamera, pd3dDevice, pd3dDeviceContext) {

}
CPlaneRenderContainer::~CPlaneRenderContainer() {

}