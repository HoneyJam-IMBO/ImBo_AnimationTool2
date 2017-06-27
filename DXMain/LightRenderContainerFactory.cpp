#include "stdafx.h"
#include "LightRenderContainerFactory.h"

void CLightRenderContainerFactory::CreateContainer() {
	m_pContainer = new CRenderContainer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
};

void CLightRenderContainerFactory::CreateMesh() {

	//--------------------------------메쉬 제작------------------------
	shared_ptr<CMesh> pMesh = make_shared<CTestMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	m_pContainer->SetMesh(pMesh);//mesh set과 동시에 instancing buffer 생성 및 set
	//--------------------------------메쉬 제작------------------------


};
void CLightRenderContainerFactory::CreateSharedBuffer(int nMaxObjects) {
	m_pContainer->CreateSharedBuffer(nMaxObjects, sizeof(VS_VB_INSTANCE));
}
void CLightRenderContainerFactory::CreateShader() {
	//--------------------------------쉐이더 제작-----------------------
	shared_ptr<CShader> pShader = make_shared<CTestShader>(m_pd3dDevice, m_pd3dDeviceContext);
	pShader->Begin();
	m_pContainer->SetShader(pShader);
	//--------------------------------쉐이더 제작-----------------------

};
void CLightRenderContainerFactory::CreateTexture() {
	
};
void CLightRenderContainerFactory::CreateMaterial() {
	//-------------------------------재질 제작-----------------------------
	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>(m_pd3dDevice, m_pd3dDeviceContext);
	pMaterial->Begin();
	pMaterial->SetMaterialInfo(XMFLOAT4(0.5, 0.5, 0.5, 1.0), 4, 1);
	m_pContainer->SetMaterial(pMaterial);
	//-------------------------------재질 제작-----------------------------
};