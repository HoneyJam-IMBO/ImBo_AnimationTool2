#include "stdafx.h"
#include "LightRenderContainer.h"

//---------------------------dxobject---------------------------------
bool CLightRenderContainer::Begin() {
	
	//-------------------------------part container 제작--------------------
	CreateDirectionalContainer(1);
	CreatePointContainer(100);
	CreateSpotContainer(100);
	CreateCapsuleContainer(100);
	//-------------------------------part container 제작--------------------

	CreateShaderState();

	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	

	return true;
}

void CLightRenderContainer::CreateDirectionalContainer(int nMaxObjects) {
	CRenderContainer* pRenderContainer = new CRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);

	//--------------------------------메쉬 제작------------------------
	shared_ptr<CMesh> pMesh = make_shared<CDirectionalLightMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	pRenderContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 se
	//--------------------------------메쉬 제작------------------------

	//--------------------------------쉐이더 제작-----------------------
	shared_ptr<CRenderShader> pShader= make_shared<CDirectionalLightShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->CreateVS(TEXT("VSDirectionalLight.cso"));
	pShader->CreatePS(TEXT("PSDirectionalLight.cso"));
	pShader->Begin();
	pRenderContainer->SetShader(pShader);
	//--------------------------------쉐이더 제작-----------------------

	//--------------------------------버퍼 제작--------------------------
	shared_ptr<CConstantBuffer> pConstantBuffer = make_shared<CConstantBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	UINT pSlot[1] = { PS_OBJECT_BUFFER_SLOT };
	UINT pBufferStrides[1] = { sizeof(DIRECTIONAL_AMBIENT_LIGHT) };
	UINT pBindFlags[1] = { BIND_PS };
	UINT pObjects[1] = { nMaxObjects };
	UINT pOffsets[1] = { 0 };


	pConstantBuffer->Begin(1, pSlot, pObjects, pBufferStrides, pBindFlags, pOffsets);
	pRenderContainer->SetBuffer(pConstantBuffer);
	//--------------------------------버퍼 제작----------------------------

	pRenderContainer->Begin();
	m_mLightRenderContainer.insert(pairLightRenderContainer(light_id::LIGHT_DIRECTIONAL, pRenderContainer));
}
void CLightRenderContainer::CreatePointContainer(int nMaxObjects) {

	//point light
	CRenderContainer* pLightRenderContainer = new CRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);

	//--------------------------------메쉬 제작------------------------
	shared_ptr<CMesh> pMesh = make_shared<CPointLightMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	pLightRenderContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	//--------------------------------메쉬 제작------------------------

	//--------------------------------쉐이더 제작-----------------------
	shared_ptr<CRenderShader> pShader = make_shared<CPointLightShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->CreateVS(TEXT("VSPointLight.cso"));
	pShader->CreateHS(TEXT("HSPointLight.cso"));
	pShader->CreateDS(TEXT("DSPointLight.cso"));
	pShader->CreatePS(TEXT("PSPointLight.cso"));
	pShader->Begin();
	pLightRenderContainer->SetShader(pShader);
	//--------------------------------쉐이더 제작-----------------------

	//--------------------------------버퍼 제작--------------------------
	//pLightRenderContainer->CreateSharedBuffer(nMaxObjects, sizeof(POINT_LIGHT_CB));
	shared_ptr<CConstantBuffer> pConstantBuffer = make_shared<CConstantBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	UINT pSlot[2] = { DS_OBJECT_BUFFER_SLOT, PS_OBJECT_BUFFER_SLOT };
	UINT pBufferStrides[2] = { sizeof(POINT_LIGHT_DS_CB), sizeof(POINT_LIGHT_PS_CB) };
	UINT pBindFlags[2] = { BIND_DS, BIND_PS };
	UINT pObjects[2] = { nMaxObjects, nMaxObjects };
	UINT pOffsets[2] = { 0, 0 };

	pConstantBuffer->Begin(2, pSlot, pObjects, pBufferStrides, pBindFlags, pOffsets);
	pLightRenderContainer->SetBuffer(pConstantBuffer);
	//--------------------------------버퍼 제작----------------------------

	pLightRenderContainer->Begin();
	m_mLightRenderContainer.insert(pairLightRenderContainer(light_id::LIGHT_POINT, pLightRenderContainer));
	//point light

}
void CLightRenderContainer::CreateSpotContainer(int nMaxObjects) {

	//boost
	CRenderContainer* pLightRenderContainer = new CRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);

	//--------------------------------메쉬 제작------------------------
	shared_ptr<CMesh> pMesh = make_shared<CSpotLightMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	pLightRenderContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	//--------------------------------메쉬 제작------------------------

	//--------------------------------쉐이더 제작-----------------------
	shared_ptr<CRenderShader> pShader = make_shared<CSpotLightShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->CreateVS(TEXT("VSSpotLight.cso"));
	pShader->CreateHS(TEXT("HSSpotLight.cso"));
	pShader->CreateDS(TEXT("DSSpotLight.cso"));
	pShader->CreatePS(TEXT("PSSpotLight.cso"));

	pShader->Begin();
	pLightRenderContainer->SetShader(pShader);
	//--------------------------------쉐이더 제작-----------------------

	//--------------------------------버퍼 제작--------------------------
	//pLightRenderContainer->CreateSharedBuffer(nMaxObjects, sizeof(SPOT_LIGHT_CB));
	shared_ptr<CConstantBuffer> pConstantBuffer = make_shared<CConstantBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	UINT pSlot[2] = { DS_OBJECT_BUFFER_SLOT, PS_OBJECT_BUFFER_SLOT };
	UINT pBufferStrides[2] = { sizeof(SPOT_LIGHT_DS_CB), sizeof(SPOT_LIGHT_PS_CB) };
	UINT pBindFlags[2] = { BIND_DS, BIND_PS };
	UINT pObjects[2] = { nMaxObjects, nMaxObjects };
	UINT pOffsets[2] = { 0, 0 };

	pConstantBuffer->Begin(2, pSlot, pObjects, pBufferStrides, pBindFlags, pOffsets);
	pLightRenderContainer->SetBuffer(pConstantBuffer);
	//--------------------------------버퍼 제작----------------------------

	pLightRenderContainer->Begin();
	m_mLightRenderContainer.insert(pairLightRenderContainer(light_id::LIGHT_SPOT, pLightRenderContainer));
	//boost
}
void CLightRenderContainer::CreateCapsuleContainer(int nMaxObjects) {

	//adaptor
	CRenderContainer* pLightRenderContainer = new CRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);

	//--------------------------------메쉬 제작------------------------
	shared_ptr<CMesh> pMesh = make_shared<CCapsuleLightMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	pLightRenderContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	//--------------------------------메쉬 제작------------------------

	//--------------------------------쉐이더 제작-----------------------
	shared_ptr<CRenderShader> pShader = make_shared<CCapsuleLightShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->CreateVS(TEXT("VSCapsuleLight.cso"));
	pShader->CreateHS(TEXT("HSCapsuleLight.cso"));
	pShader->CreateDS(TEXT("DSCapsuleLight.cso"));
	pShader->CreatePS(TEXT("PSCapsuleLight.cso"));

	pShader->Begin();
	pLightRenderContainer->SetShader(pShader);
	//--------------------------------쉐이더 제작-----------------------

	//--------------------------------버퍼 제작-----------------------------
	shared_ptr<CConstantBuffer> pConstantBuffer = make_shared<CConstantBuffer>(m_pd3dDevice, m_pd3dDeviceContext);
	UINT pSlot[2] = { DS_OBJECT_BUFFER_SLOT, PS_OBJECT_BUFFER_SLOT };
	UINT pBufferStrides[2] = { sizeof(CAPSULE_LIGHT_DS_CB) , sizeof(CAPSULE_LIGHT_PS_CB) };
	UINT pBindFlags[2] = { BIND_DS, BIND_PS };
	UINT pObjects[2] = { nMaxObjects, nMaxObjects };
	UINT pOffsets[2] = { 0, 0 };

	pConstantBuffer->Begin(2, pSlot, pObjects, pBufferStrides, pBindFlags, pOffsets);
	pLightRenderContainer->SetBuffer(pConstantBuffer);

	//pLightRenderContainer->CreateSharedBuffer(nMaxObjects, sizeof(CAPSULE_LIGHT_CB));
	//--------------------------------버퍼 제작-----------------------------

	pLightRenderContainer->Begin();
	m_mLightRenderContainer.insert(pairLightRenderContainer(light_id::LIGHT_CAPSULE, pLightRenderContainer));

}

void CLightRenderContainer::CreateShaderState() {
	//depth stencil state
	D3D11_DEPTH_STENCIL_DESC descDepthStencil;
	ZeroMemory(&descDepthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));
	descDepthStencil.DepthEnable = true;
	descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepthStencil.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	m_pd3dDevice->CreateDepthStencilState(&descDepthStencil, &m_pLightDepthStencilState);
	//depth stencil state

	//rasterizer state
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	//descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_FRONT;
	m_pd3dDevice->CreateRasterizerState(&descRasterizer, &m_pLightRasterizerState);
	//rasterizer state

	//blend state
	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.RenderTarget[0].BlendEnable = true;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//blend state
	m_pd3dDevice->CreateBlendState(&descBlend, &m_pLightBlendState);
}
bool CLightRenderContainer::End() {
	for (auto p = m_lpObjects.begin(); p != m_lpObjects.end(); ++p) {
		(*p)->End();
	}
	m_lpObjects.clear();

	return CRenderContainer::End();
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CLightRenderContainer::UpdateShaderState() {

	//---------------------------lights render 준비!--------------------------------
	CLight* pLight = nullptr;
	//----------------------------lights Add----------------------------------
	for (auto pObject : m_lpObjects) {
		pLight = (CLight*)pObject;//강제 형변환.. ㅠ
		m_mLightRenderContainer[pLight->GetLightID()]->AddObject(pLight);
	}
	//-----------------------------parts Add----------------------------------------
	//---------------------------lights render 준비!--------------------------------
}
void CLightRenderContainer::SetShaderState() {
	//--------------------------------deferred lighting--------------------------------
	ID3D11Buffer* pGBufferUnpackingBuffer = m_pCamera->GetGBufferUnpackingBuffer();
	m_pd3dDeviceContext->PSSetConstantBuffers(PS_UNPACKING_SLOT, 1, &pGBufferUnpackingBuffer);

	ID3D11Buffer* pViewProjectionBuffer = m_pCamera->GetViewProjectionBuffer();
	m_pd3dDeviceContext->PSSetConstantBuffers(PS_CAMERA_DATA_SLOT, 1, &pViewProjectionBuffer);


	if (m_pTexture)m_pTexture->SetShaderState();
}
void CLightRenderContainer::RenderExcute() {
	//이전 상태 저장
	m_pd3dDeviceContext->OMGetDepthStencilState(&m_pPreDepthStencilState, &m_PreStencilRef);
	m_pd3dDeviceContext->RSGetState(&m_pPreRasterizerState);
	m_pd3dDeviceContext->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);

	light_id id = light_id::LIGHT_END;
	//scene의 모든 Part의 rendercontainer안에 part list Render!
	for (int i = 0; i < light_id::LIGHT_END; ++i) {
		id = (light_id)i;
		if (id == light_id::LIGHT_DIRECTIONAL) {
			//directional
			m_mLightRenderContainer[id]->Render();

			m_pd3dDeviceContext->OMSetBlendState(m_pLightBlendState, nullptr, 0xffffffff);
			m_pd3dDeviceContext->OMSetDepthStencilState(m_pLightDepthStencilState, 0);
			m_pd3dDeviceContext->RSSetState(m_pLightRasterizerState);
		}
		else {
			m_mLightRenderContainer[id]->Render();
		}

	}

	
}

void CLightRenderContainer::CleanShaderState() {
	//이전 상태로 되돌림
	m_pd3dDeviceContext->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);
	m_pd3dDeviceContext->OMSetDepthStencilState(m_pPreDepthStencilState, m_PreStencilRef);
	m_pd3dDeviceContext->RSSetState(m_pPreRasterizerState);
	
	if (m_pTexture) m_pTexture->CleanShaderState();

	light_id id = light_id::LIGHT_END;
	//scene의 모든 Part의 rendercontainer안에 part list Clear!
	for (int i = 0; i < light_id::LIGHT_END; ++i) {
		id = (light_id)i;
		m_mLightRenderContainer[id]->ClearObjectList();
	}

}

CLightRenderContainer::CLightRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderContainer(pCamera, pd3dDevice, pd3dDeviceContext) {

}
CLightRenderContainer::~CLightRenderContainer() {

}