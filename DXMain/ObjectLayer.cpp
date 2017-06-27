#include "stdafx.h"
#include "ObjectLayer.h"
//#include "RenderContainerSeller.h"

//---------------------------dxobject---------------------------------
bool CObjectLayer::Begin() {
	
	CreateShaderState();

	for (int i = 0; i < object_id::OBJECT_END; ++i) {
		object_id id = (object_id)i;
		m_mRenderContainer.insert(pairRenderContainer(id, RCSELLER->GetRenderContainer(id)));
	}

	
	return true;
}

void CObjectLayer::CreateShaderState() {
	//skybox depth stencil
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = FALSE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = TRUE;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC noSkyStencilOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_EQUAL };
	descDepth.FrontFace = noSkyStencilOp;
	descDepth.BackFace = noSkyStencilOp;
	m_pd3dDevice->CreateDepthStencilState(&descDepth, &m_pd3dDepthStencilState);
	//DXUT_SetDebugName(m_pSkyNoDepthStencilMaskState, "Sky No Depth Stencil Mask DS");
	//
	//m_pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dDepthStencilState);

	//RS
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	//descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	m_pd3dDevice->CreateRasterizerState(&descRasterizer, &m_pd3dSpaceRSState);
	//RS
}
bool CObjectLayer::End() {
	
	m_mRenderContainer.clear();

	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CObjectLayer::UpdateShaderState(shared_ptr<CCamera> pCamera) {

}
void CObjectLayer::SetShaderState(shared_ptr<CCamera> pCamera) {
	
}
void CObjectLayer::RenderExcute(shared_ptr<CCamera> pCamera) {
	
	//skybox
	m_pd3dDeviceContext->OMGetDepthStencilState(&m_pd3dTempDepthStencilState, &m_TempStencil);
	m_pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);
	m_mRenderContainer[object_id::OBJECT_SKYBOX]->Render(pCamera);
	m_pd3dDeviceContext->OMSetDepthStencilState(m_pd3dTempDepthStencilState, m_TempStencil);
	//skybox

	//space
	//m_pd3dDeviceContext->RSGetState(&m_pd3dTempRSState);
	//m_pd3dDeviceContext->RSSetState(m_pd3dSpaceRSState);
	//m_mRenderContainer[object_id::OBJECT_SPACE]->Render();
	//m_pd3dDeviceContext->RSSetState(m_pd3dTempRSState);
	//space

	//terrain
	//m_pd3dDeviceContext->RSGetState(&m_pd3dTempRSState);
	//m_pd3dDeviceContext->RSSetState(m_pd3dSpaceRSState);
	//m_mRenderContainer[object_id::OBJECT_TERRAIN]->Render(pCamera);
	//m_pd3dDeviceContext->RSSetState(m_pd3dTempRSState);
	//terrain

	object_id id = object_id::OBJECT_END;
	//scene의 모든 Part의 rendercontainer안에 part list Render!
	for (int i = 3; i < object_id::OBJECT_END; ++i) {
		id = (object_id)i;
		m_mRenderContainer[id]->Render(pCamera);
	}
}

void CObjectLayer::CleanShaderState(shared_ptr<CCamera> pCamera) {
	
	object_id id = object_id::OBJECT_END;
	//scene의 모든 Part의 rendercontainer안에 part list Clear!
	for (int i = 0; i < object_id::OBJECT_END; ++i) {
		id = (object_id)i;
		m_mRenderContainer[id]->ClearObjectList();
	}

}

CObjectLayer::CObjectLayer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CLayer(pd3dDevice, pd3dDeviceContext) {

}
CObjectLayer::~CObjectLayer() {

}