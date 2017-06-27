#include "stdafx.h"
#include "CoreRenderContainer.h"

//---------------------------dxobject---------------------------------
bool CCoreRenderContainer::Begin() {
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
	//-------------------------------part container 제작--------------------
	CreateCoreContainer(100);
	CreateWeaponContainer(1000);
	CreateBoostContainer(1000);
	CreateAdaptorContainer(1000);
	//-------------------------------part container 제작--------------------

	return true;
}
void CCoreRenderContainer::CreateCoreContainer(int nMaxObjects) {
	//CRenderContainer* pRenderContainer = new CInstancingObjectRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	//
	////--------------------------------메쉬 제작------------------------
	////shared_ptr<CMesh> pMesh = make_shared<CCoreMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	//shared_ptr<CMesh> pMesh = make_shared<CTestMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	//pMesh->Begin();
	//pRenderContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	////--------------------------------메쉬 제작------------------------
	//
	////--------------------------------쉐이더 제작-----------------------
	//pRenderContainer->SetShader(m_pShader);
	////--------------------------------쉐이더 제작-----------------------
	////-------------------------------재질 제작-----------------------------
	//shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	//pMaterial->Begin();
	//pMaterial->SetMaterialInfo(XMFLOAT4(0.5, 0.1, 0.1, 1.0), 4, 1);
	//pRenderContainer->SetMaterial(pMaterial);
	////-------------------------------재질 제작-----------------------------
	//
	////-------------------------------버퍼 제작-----------------------------
	//shared_ptr<CInstanceBuffer> pConstantBuffer = make_shared<CInstanceBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	//UINT pBufferStrides[1] = { sizeof(VS_VB_INSTANCE) };
	//UINT pObjects[1] = { nMaxObjects };
	//UINT pOffsets[1] = { 0 };
	//pConstantBuffer->SetInstanceBufferData(1, pObjects, pBufferStrides, pOffsets);
	//pRenderContainer->SetInstanceBuffer(pConstantBuffer);
	////-------------------------------버퍼 제작-----------------------------
	//
	//pRenderContainer->Begin();
	//m_mPartRenderContainer.insert(pairPartRenderContainer(part_id::PART_CORE, pRenderContainer));
}
void CCoreRenderContainer::CreateWeaponContainer(int nMaxObjects) {

	////weapon
	//CRenderContainer* pPartRenderContainer = new CInstancingObjectRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	//
	////--------------------------------메쉬 제작------------------------

	////shared_ptr<CMesh> pMesh = make_shared<CWeaponMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	//shared_ptr<CMesh> pMesh = make_shared<CTestMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	//pMesh->Begin();
	//pPartRenderContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	////--------------------------------메쉬 제작------------------------
	//
	////--------------------------------쉐이더 제작-----------------------
	//pPartRenderContainer->SetShader(m_pShader);
	////--------------------------------쉐이더 제작-----------------------
	////-------------------------------재질 제작-----------------------------
	//shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	//pMaterial->Begin();
	//pMaterial->SetMaterialInfo(XMFLOAT4(0.1, 0.7, 0.0, 1.0), 4, 1);
	//pPartRenderContainer->SetMaterial(pMaterial);
	////-------------------------------재질 제작-----------------------------
	//
	////-------------------------------버퍼 제작-----------------------------
	//shared_ptr<CInstanceBuffer> pConstantBuffer = make_shared<CInstanceBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	//UINT pBufferStrides[1] = { sizeof(VS_VB_INSTANCE) };
	//UINT pObjects[1] = { nMaxObjects };
	//UINT pOffsets[1] = { 0 };
	//pConstantBuffer->SetInstanceBufferData(1, pObjects, pBufferStrides, pOffsets);
	//pPartRenderContainer->SetInstanceBuffer(pConstantBuffer);
	////-------------------------------버퍼 제작-----------------------------
	//
	//pPartRenderContainer->Begin();
	//m_mPartRenderContainer.insert(pairPartRenderContainer(part_id::PART_WEAPON, pPartRenderContainer));
	////weapon

}
void CCoreRenderContainer::CreateBoostContainer(int nMaxObjects) {

	////boost
	//CRenderContainer* pPartRenderContainer = new CInstancingObjectRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	//
	////--------------------------------메쉬 제작------------------------
	////shared_ptr<CMesh> pMesh = make_shared<CBoostMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	//shared_ptr<CMesh> pMesh = make_shared<CTestMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	//pMesh->Begin();
	//pPartRenderContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	////--------------------------------메쉬 제작------------------------
	//
	////--------------------------------쉐이더 제작-----------------------
	//pPartRenderContainer->SetShader(m_pShader);
	////--------------------------------쉐이더 제작-----------------------
	////-------------------------------재질 제작-----------------------------
	//shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	//pMaterial->Begin();
	//pMaterial->SetMaterialInfo(XMFLOAT4(0.1, 0.6, 0.6, 1.0), 4, 1);
	//pPartRenderContainer->SetMaterial(pMaterial);
	////-------------------------------재질 제작-----------------------------
	//
	////-------------------------------버퍼 제작-----------------------------
	//shared_ptr<CInstanceBuffer> pConstantBuffer = make_shared<CInstanceBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	//UINT pBufferStrides[1] = { sizeof(VS_VB_INSTANCE) };
	//UINT pObjects[1] = { nMaxObjects };
	//UINT pOffsets[1] = { 0 };
	//pConstantBuffer->SetInstanceBufferData(1, pObjects, pBufferStrides, pOffsets);
	//pPartRenderContainer->SetInstanceBuffer(pConstantBuffer);
	////-------------------------------버퍼 제작-----------------------------
	//
	//pPartRenderContainer->Begin();
	//m_mPartRenderContainer.insert(pairPartRenderContainer(part_id::PART_BOOST, pPartRenderContainer));
	//boost
}
void CCoreRenderContainer::CreateAdaptorContainer(int nMaxObjects) {

	////adaptor
	//CRenderContainer* pPartRenderContainer = new CInstancingObjectRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	//
	////--------------------------------메쉬 제작------------------------
	////shared_ptr<CMesh> pMesh = make_shared<CAdaptorMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	//shared_ptr<CMesh> pMesh = make_shared<CTestMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	//pMesh->Begin();
	//pPartRenderContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	////--------------------------------메쉬 제작------------------------
	//
	////--------------------------------쉐이더 제작-----------------------
	//pPartRenderContainer->SetShader(m_pShader);
	////--------------------------------쉐이더 제작-----------------------
	////-------------------------------재질 제작-----------------------------
	//shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	//pMaterial->Begin();
	//pMaterial->SetMaterialInfo(XMFLOAT4(0.6, 0.5, 0.1, 1.0), 4, 1);
	//pPartRenderContainer->SetMaterial(pMaterial);
	////-------------------------------재질 제작-----------------------------
	//
	////-------------------------------버퍼 제작-----------------------------
	//shared_ptr<CInstanceBuffer> pConstantBuffer = make_shared<CInstanceBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	//UINT pBufferStrides[1] = { sizeof(VS_VB_INSTANCE) };
	//UINT pObjects[1] = { nMaxObjects };
	//UINT pOffsets[1] = { 0 };
	//pConstantBuffer->SetInstanceBufferData(1, pObjects, pBufferStrides, pOffsets);
	//pPartRenderContainer->SetInstanceBuffer(pConstantBuffer);
	////-------------------------------버퍼 제작-----------------------------
	//
	//pPartRenderContainer->Begin();
	//m_mPartRenderContainer.insert(pairPartRenderContainer(part_id::PART_ADAPTOR, pPartRenderContainer));

}
bool CCoreRenderContainer::End() {
	for (auto p = m_lpObjects.begin(); p != m_lpObjects.end(); ++p) {
		(*p)->End();
	}
	m_lpObjects.clear();

	return CRenderContainer::End();
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CCoreRenderContainer::UpdateShaderState() {

	////---------------------------parts render 준비!--------------------------------
	////----------------------------parts list 제작----------------------------------
	//list<CPart*> lParts;
	//for (auto pObject : m_lpObjects) {
	//	CCore* pCore = (CCore*)pObject;
	//	pCore->GetPartsList(lParts);
	//}
	////-----------------------------parts Add----------------------------------------
	//for (auto pParts : lParts) {
	//	m_mPartRenderContainer[pParts->GetPartID()]->AddObject(pParts);
	//}
	////---------------------------parts render 준비!--------------------------------
}
void CCoreRenderContainer::SetShaderState() {

}
void CCoreRenderContainer::RenderExcute() {
	//part_id id = part_id::PART_END;
	////scene의 모든 Part의 rendercontainer안에 part list Render!
	//for (int i = 0; i < part_id::PART_END; ++i) {
	//	id = (part_id)i;
	//	m_mPartRenderContainer[id]->Render();
	//}

}

void CCoreRenderContainer::CleanShaderState() {

	//part_id id = part_id::PART_END;
	////scene의 모든 Part의 rendercontainer안에 part list Clear!
	//for (int i = 0; i < part_id::PART_END; ++i) {
	//	id = (part_id)i;
	//	m_mPartRenderContainer[id]->ClearObjectList();
	//}

}

CCoreRenderContainer::CCoreRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderContainer(pCamera, pd3dDevice, pd3dDeviceContext) {

}
CCoreRenderContainer::~CCoreRenderContainer() {

}