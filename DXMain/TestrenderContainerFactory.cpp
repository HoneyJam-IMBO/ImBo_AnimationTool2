#include "stdafx.h"
#include "TestRenderContainerFactory.h"

void CTestRenderContainerFactory::CreateContainer() {
	m_pContainer = new CRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
};

void CTestRenderContainerFactory::CreateMesh() {

	//--------------------------------메쉬 제작------------------------
	shared_ptr<CMesh> pMesh = make_shared<CTestMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	m_pContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	//--------------------------------메쉬 제작------------------------
	
	
};
void CTestRenderContainerFactory::CreateSharedBuffer(int nMaxObjects) {
	m_pContainer->CreateSharedBuffer(nMaxObjects, sizeof(VS_VB_INSTANCE));
}
void CTestRenderContainerFactory::CreateShader() {
	//--------------------------------쉐이더 제작-----------------------
	shared_ptr<CShader> pShader = make_shared<CTestShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->Begin();
	m_pContainer->SetShader(pShader);
	//--------------------------------쉐이더 제작-----------------------

};
void CTestRenderContainerFactory::CreateTexture() {
	//-------------------------------텍스쳐 제작-----------------------------
	shared_ptr<CTexture> pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	pTexture->Begin();
	pTexture->SetTexture(PS_TEXTURE, _T("Brick01.jpg"));
	pTexture->SetSampler(PS_TEXTURE_SAMPLER, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_COMPARISON_NEVER, 0, 0);
	m_pContainer->SetTexture(pTexture);
	//-------------------------------텍스쳐 제작-----------------------------

};
void CTestRenderContainerFactory::CreateMaterial() {
	//-------------------------------재질 제작-----------------------------
	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	pMaterial->Begin();
	pMaterial->SetMaterialInfo(XMFLOAT4(0.5, 0.5, 0.5, 1.0), 4, 1);
	m_pContainer->SetMaterial(pMaterial);
	//-------------------------------재질 제작-----------------------------
};