#include "stdafx.h"
#include "TestAnimateContainer.h"
#include "stdafx.h"
#include "AnimateContainer.h"

//---------------------------dxobject---------------------------------
bool CTestAnimateContainer::Begin(CRenderContainerSeller* pSeller) {
	int nMaxObjects = 10;
	//--------------------------------按眉 力累------------------------
	//CTestObject* pObject = nullptr;
	//for (int i = 0; i < nMaxObjects; ++i) {
	//	pObject = new CTestObject();
	//	pObject->Begin();
	//	pObject->SetPosition(XMLoadFloat3(&XMFLOAT3(i * 50, 5, 0)));
	//	pObject->SetRenderContainer(pSeller);
	//
	//	AddObject(pObject);
	//}
	CPlane* pPlaneObject = new CPlane();
	pPlaneObject ->Begin();
	pPlaneObject ->SetPosition(XMVectorSet(500, 0, 500, 0.f));
	//pPlaneObject->SetRenderContainer(pSeller);
	AddObject(pPlaneObject);


	nMaxObjects = 10;
	CCore* pCore = nullptr;
	CPart* pParts = nullptr;
	for (int i = 0; i < nMaxObjects; ++i) {
		pCore = new CCore();
		
	
		pCore->Begin();
		pCore->SetPosition(XMVectorSet(i * 50.0f, 10.0f, 0.0f, 0.f));
	
		pParts = new CWeaponPart();
		pParts->Begin();
		//pParts->SetRenderContainer(pSeller);
		pCore->Equip(rand()%6, pParts);
		
		pParts = new CBoostPart();
		pParts->Begin();
		//pParts->SetRenderContainer(pSeller);
		pCore->Equip(rand() % 6, pParts);
	
		pParts = new CAdaptorPart();
		pParts->Begin();
		//pParts->SetRenderContainer(pSeller);
		if (pCore->Equip(rand() % 6, pParts)) {
			CPart* pAdaptorParts = new CBoostPart();
			pAdaptorParts->Begin();
			//pAdaptorParts->SetRenderContainer(pSeller);
			pParts->Equip(rand() % 6, pAdaptorParts);
		}
	
		
		//pCore->SetRenderContainer(pSeller);
		AddObject(pCore);
	}

	CDirectionalLight* pDirectionalLight = nullptr;
	pDirectionalLight = new CDirectionalLight;
	pDirectionalLight->Begin(DIRECTIONAL_AMBIENT_LIGHT{
		XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f),XMFLOAT4(0.0f, 0.1f, 0.1f, 0.0f) , XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f),//dir
		XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(5.1f, 5.1f, 5.1f, 1.0f)//ambient
	});
	//pDirectionalLight->SetRenderContainer(pSeller);
	AddObject(pDirectionalLight);

	

	CPointLight* pPointLight = nullptr;
	int nPointLight = 10;
	float fStartX = -500;
	float fStartZ = -500;
	for (int i = 0; i < nPointLight; ++i) {
		for (int j = 0; j < nPointLight; ++j) {
			pPointLight = new CPointLight;
			pPointLight->Begin(POINT_LIGHT{ 85.0f, XMFLOAT3(static_cast<float>(rand() % 5), static_cast<float>(rand() % 5), static_cast<float>(rand() % 5))});
			
			pPointLight->SetPosition(XMVectorSet(fStartX + i * 100, 50, fStartZ + j * 100, 0.f));
			//pPointLight->SetRenderContainer(pSeller);
			AddObject(pPointLight);
		}
		
	}
	
	fStartX += 50;
	fStartZ += 50;
	CCapsuleLight* pCapsuleLight = nullptr;
	int nCapsuleLight = 10;
	
	for (int i = 0; i < nCapsuleLight; ++i) {
		for (int j = 0; j < nCapsuleLight; ++j) {
			pCapsuleLight = new CCapsuleLight;
			pCapsuleLight->Begin(CAPSULE_LIGHT{ 50.0f, XMFLOAT3(static_cast<float>(rand() % 5), static_cast<float>(rand() % 5), static_cast<float>(rand() % 5)), 20.0f});
	
			pCapsuleLight->SetPosition(XMVectorSet(fStartX + i * 100, 10, fStartZ + j * 100, 0.f));
			//pCapsuleLight->SetRenderContainer(pSeller);
			AddObject(pCapsuleLight);
		}
	
	}
	
	//fStartX += 50;
	//fStartZ += 50;
	
	int nSpotLight = 10;
	CSpotLight* pSpotLight = nullptr;
	for (int i = 0; i < nSpotLight; ++i) {
		for (int j = 0; j < nSpotLight; ++j) {
			pSpotLight = new CSpotLight;
			pSpotLight->Begin(SPOT_LIGHT{ 125.0f, XMFLOAT3(static_cast<float>(rand() % 100), static_cast<float>(rand() % 100), static_cast<float>(rand() % 100)), 50.0f, 5.0f });
			pSpotLight->SetPosition(XMVectorSet(fStartX + i * 100, 25, fStartZ + j * 100, 0.f));
			//pSpotLight->SetRenderContainer(pSeller);
			AddObject(pSpotLight);
		}
	}
	//--------------------------------按眉 力累-------------------------
	

	return true;
}
bool CTestAnimateContainer::End() {
	for (auto p : m_lpObjects) {
		p->End();
	}
	m_lpObjects.clear();
	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------

void CTestAnimateContainer::Animate(float fTimeElapsed, CAnimateContainer* pExcludeContainer) {
	//葛电 按眉 Animate
	for (auto p : m_lpObjects) {
		p->Animate(fTimeElapsed);
	}
}


CTestAnimateContainer::CTestAnimateContainer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CAnimateContainer(pd3dDevice, pd3dDeviceContext) {

}
CTestAnimateContainer::~CTestAnimateContainer() {

}