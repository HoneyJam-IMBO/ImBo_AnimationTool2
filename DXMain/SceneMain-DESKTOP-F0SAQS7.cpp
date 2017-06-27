#include "stdafx.h"
#include "SceneMain.h"

bool CSceneMain::Begin() {

	//m_pPlayer = new CPlayer;
	//m_pPlayer->Begin();
	//----------------------------------camera-------------------------------------
	m_pCamera = m_pFrameWork->GetCamera();
	//----------------------------------camera-------------------------------------

	//--------------------------------container----------------------------
	m_RenderContainerSeller = new CRenderContainerSeller(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	m_RenderContainerSeller->Begin();
	//--------------------------------container----------------------------
	
	//ㅋㅋ 나중에 밖으로 빼자.. debuger init
	DEBUGER->Begin(m_pd3dDevice, m_pd3dDeviceContext, m_RenderContainerSeller, m_pFrameWork);
	//ㅋㅋ 나중에 밖으로 빼자.. debuger init

	//--------------------------------------space-------------------------------------
	//space
	m_pSpaceContainer = new CSpaceContainer();
	m_pSpaceContainer->Begin(m_RenderContainerSeller, SPACE_SIZE, SPACE_LEVEL);
	//terrain
	m_pTerrainContainer = new CTerrainContainer();
	m_pTerrainContainer->Begin(L"../../Assets/HeightMap.raw", 257, 257, 1, m_RenderContainerSeller, m_pSpaceContainer);
	//--------------------------------------space-------------------------------------

	//--------------------------------객체 제작------------------------
	int nMaxObjects = 1;
	CTestObject* pObject = nullptr;
	for (int i = 0; i < nMaxObjects; ++i) {
		for (int j = 0; j< nMaxObjects; ++j) {
			pObject = new CTestObject();
			pObject->Begin();
			pObject->SetTerrainContainer(m_pTerrainContainer);
			pObject->SetRenderContainer(m_RenderContainerSeller);
			pObject->SetPosition(XMLoadFloat3(&XMFLOAT3(static_cast<float>(i * 100), 50.0f, static_cast<float>(j * 100))));

			m_pSpaceContainer->AddObject(pObject);
		}
	}
	//계층구조 객체
	
	nMaxObjects = 0;
	CCore* pCore = nullptr;
	CPart* pParts = nullptr;
	for (int i = 0; i < nMaxObjects; ++i) {
		pCore = new CCore();
	
	
		pCore->Begin();
		pCore->SetPosition(::XMLoadFloat3(&XMFLOAT3(static_cast<float>(rand()%800+100), 50.0f, static_cast<float>(rand()%800+100))));
	
		//pParts = new CWeaponPart();
		//pParts->Begin();
		//pParts->SetRenderContainer(m_RenderContainerSeller);
		//pCore->Equip(rand() % 6, pParts);
		
		pParts = new CBoostPart();
		pParts->Begin();
		pParts->SetRenderContainer(m_RenderContainerSeller);
		pCore->Equip(3, pParts);
		
		//pParts = new CAdaptorPart();
		//pParts->Begin();
		//pParts->SetRenderContainer(m_RenderContainerSeller);
		//if (pCore->Equip(rand() % 6, pParts)) {
		//	CPart* pAdaptorParts = new CBoostPart();
		//	pAdaptorParts->Begin();
		//	pAdaptorParts->SetRenderContainer(m_RenderContainerSeller);
		//	pParts->Equip(rand() % 6, pAdaptorParts);
		//}
	
	
		pCore->SetRenderContainer(m_RenderContainerSeller);
		m_pSpaceContainer->AddObject(pCore);
	}
	

	CPointLight* pPointLight = nullptr;
	int nPointLight = 5;
	float fStartX = 0;
	float fStartZ = 0;
	for (int i = 0; i < nPointLight; ++i) {
		for (int j = 0; j < nPointLight; ++j) {
			pPointLight = new CPointLight;
			pPointLight->Begin(POINT_LIGHT{ 85.0f, XMFLOAT3(static_cast<float>(rand() % 5), static_cast<float>(rand() % 5), static_cast<float>(rand() % 5)), 1.0f });
	
			pPointLight->SetPosition(::XMLoadFloat3(&XMFLOAT3(fStartX + static_cast<float>(i * 100), static_cast<float>(rand() % 50 + 125), static_cast<float>(fStartZ + j * 100))));
			pPointLight->SetRenderContainer(m_RenderContainerSeller);
			m_pSpaceContainer->AddObject(pPointLight);
		}
	
	}
	
	fStartX += 500;
	fStartZ += 500;
	CCapsuleLight* pCapsuleLight = nullptr;
	int nCapsuleLight = 5;
	
	for (int i = 0; i < nCapsuleLight; ++i) {
		for (int j = 0; j < nCapsuleLight; ++j) {
			pCapsuleLight = new CCapsuleLight;
			pCapsuleLight->Begin(CAPSULE_LIGHT{ 100.0f, XMFLOAT3(static_cast<float>(rand() % 5), static_cast<float>(rand() % 5), static_cast<float>(rand() % 5)), 50.0f, 1.0f });
	
			pCapsuleLight->SetPosition(::XMLoadFloat3(&XMFLOAT3(fStartX + static_cast<float>(i * 100), static_cast<float>(rand() % 50 + 125), fStartZ + static_cast<float>(j * 100))));
			pCapsuleLight->SetRenderContainer(m_RenderContainerSeller);
			m_pSpaceContainer->AddObject(pCapsuleLight);
		}
	
	}
	
	fStartX += 500;
	fStartZ += 500;
	
	int nSpotLight = 5;
	CSpotLight* pSpotLight = nullptr;
	for (int i = 0; i < nSpotLight; ++i) {
		for (int j = 0; j < nSpotLight; ++j) {
			pSpotLight = new CSpotLight;
			pSpotLight->Begin(SPOT_LIGHT{ 125, XMFLOAT3(static_cast<float>(rand() % 100), static_cast<float>(rand() % 100), static_cast<float>(rand() % 100)), 50.0f, 5.0f });
			pSpotLight->SetPosition(::XMLoadFloat3(&XMFLOAT3(fStartX + i * 100, rand()%50+125, fStartZ + j * 100)));
			pSpotLight->SetRenderContainer(m_RenderContainerSeller);
			m_pSpaceContainer->AddObject(pSpotLight);
		}
	}
	//--------------------------전역 객체 제작-------------------------
	//skybox
	m_pSkyBox = new CSkyBox();
	m_pSkyBox->Begin();

	m_pSkyBox->SetRenderContainer(m_RenderContainerSeller);
	m_pSkyBox->SetCamera(m_pCamera);
	//skybox

	//directional light
	m_pDirectionalLight = new CDirectionalLight;
	m_pDirectionalLight->Begin(DIRECTIONAL_AMBIENT_LIGHT{
		XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f),XMFLOAT4(0.0f, 0.1f, 0.1f, 0.0f) , XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f),//dir
		XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(5.1f, 5.1f, 5.1f, 1)//ambient
	});
	m_pDirectionalLight->SetRenderContainer(m_RenderContainerSeller);
	//directional light
	//--------------------------전역 객체 제작-------------------------
	//--------------------------------객체 제작-------------------------

	//create layer
	CreateRenderLayers();
	//create layer
	
	return true;
}

bool CSceneMain::End() {
	//m_pPlayer->End();
	//seller
	m_RenderContainerSeller->End();
	if (m_pObjectLayer) {
		m_pObjectLayer->End();
		delete m_pObjectLayer;
	}
	//layers
	if (m_pLightLayer) {
		m_pLightLayer->End();
		delete m_pLightLayer;
	}
	if (m_pPostProcessingLayer) {
		m_pPostProcessingLayer->End();
		delete m_pPostProcessingLayer;
	}
	//rtvs
	ReleaseForwardRenderTargets();
	//space
	if (m_pSpaceContainer) {
		m_pSpaceContainer->End();
		delete m_pSpaceContainer;
	}
	//terrain
	if (m_pTerrainContainer) {
		m_pTerrainContainer->End();
		delete m_pTerrainContainer;
	}

	//global object
	if (m_pSkyBox) {
		m_pSkyBox->End();
		delete m_pSkyBox;
	}
	if (m_pDirectionalLight) {
		m_pDirectionalLight->End();
		delete m_pDirectionalLight;
	}
	//카메라는 Framework에 존재하는 것이기에 End()작업을 진행하지 않는다.
	return true;
}

void CSceneMain::Animate(float fTimeElapsed) {
	//DEBUGER->AddText(100, 100, 100, YT_Color(255, 0, 0), L"TEST");

	//적응값 set
	m_pPostProcessingLayer->SetAdaptation(fTimeElapsed);

	//-----------------------------------space------------------------------
	m_pSpaceContainer->Animate(fTimeElapsed);
	//-----------------------------------space------------------------------

	//--------------------------전역 객체 animate / regist-------------------------
	//skybox camera 동기화
	m_pSkyBox->Animate(fTimeElapsed);
	//sky box 등록
	m_pSkyBox->RegistToContainer();
	//directional light 등록
	m_pDirectionalLight->RegistToContainer();
	//--------------------------전역 객체 animate / regist-------------------------
}

void CSceneMain::CreateRenderLayers() {
	//----------------------------------------Resource Desc-----------------------------------------//
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;
	//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//----------------------------------------Resource Desc-----------------------------------------//
	//----------------------------------------TextUre Desc-----------------------------------------//
	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = m_pFrameWork->GetDisplaySize().right;
	d3dTexture2DDesc.Height = m_pFrameWork->GetDisplaySize().bottom;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	//----------------------------------------TextUre Desc-----------------------------------------//
	//----------------------------------------Render Desc-----------------------------------------//
	D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dRTVDesc.Texture2D.MipSlice = 0;
	//----------------------------------------TextUre Desc-----------------------------------------//
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	//--------------------------------------Scene0 RTV Create-----------------------------------------//
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtColorSpecInt);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtColorSpecInt, &d3dSRVDesc, &m_pd3dsrvColorSpecInt);
	m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtColorSpecInt, &d3dRTVDesc, &m_pd3drtvColorSpecInt);
	//--------------------------------------Scene0 RTV Create-----------------------------------------//

	//--------------------------------------Scene1 RTV Create-----------------------------------------//
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtNormal);
	m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtNormal, &d3dRTVDesc, &m_pd3drtvNormal);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtNormal, &d3dSRVDesc, &m_pd3dsrvNormal);
	//--------------------------------------Scene1 RTV Create-----------------------------------------//

	//--------------------------------------Scene2 RTV Create-----------------------------------------//
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSpecPow);
	m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtSpecPow, &d3dRTVDesc, &m_pd3drtvSpecPow);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtSpecPow, &d3dSRVDesc, &m_pd3dsrvSpecPow);
	//--------------------------------------Scene2 RTV Create-----------------------------------------//


	//자기 texture set -> sampler set위함

	//---------------------make texture---------------------
	//texture set to light rendercontainer
	ID3D11ShaderResourceView *pd3dSRV= { m_pd3dsrvColorSpecInt };
	UINT Slot = { 0 };
	UINT BindFlag = { BIND_PS };
	//make sampler
	shared_ptr<CSampler> pSampler = make_shared<CSampler>(m_pd3dDevice, m_pd3dDeviceContext);
	pSampler->Begin();
	shared_ptr<CTexture> pTexture1 = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	pTexture1->Begin(pd3dSRV, pSampler, Slot, BindFlag);

	pd3dSRV = { m_pd3dsrvNormal};
	Slot = { 1 };
	BindFlag = { BIND_PS };
	shared_ptr<CTexture> pTexture2 = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	pTexture2->Begin(pd3dSRV, pSampler, Slot, BindFlag);

	pd3dSRV = { m_pd3dsrvSpecPow };
	Slot = { 2 };
	BindFlag = { BIND_PS };
	shared_ptr<CTexture> pTexture3 = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	pTexture3->Begin(pd3dSRV, pSampler, Slot, BindFlag);
	//---------------------make texture---------------------

	//set texture
	//craete object layer
	m_pObjectLayer = new CObjectLayer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	m_pObjectLayer->Begin(m_RenderContainerSeller);
	//craete object layer

	//craete light layer
	m_pLightLayer = new CLightLayer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	shared_ptr<CTexture> ppTexture[3] = { pTexture1 ,pTexture2 ,pTexture3 };
	m_pLightLayer->Begin(m_RenderContainerSeller, 3, ppTexture);
	//craete light layer

	//light texture제작
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
	m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);

	shared_ptr<CTexture> pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//make texture
	UINT LightTexSlot = { 0 };
	UINT LightTexBindFlag = { BIND_PS | BIND_CS };
	//make sampler
	shared_ptr<CSampler> pLightTexSampler = make_shared<CSampler>(m_pd3dDevice, m_pd3dDeviceContext);
	UINT LightTexSamplerBindFlag = { BIND_PS | BIND_CS };
	UINT LightTexSamplerSlot = { 0 };
	pLightTexSampler->Begin(LightTexSamplerSlot, LightTexSamplerBindFlag);
	pTexture->Begin(m_pd3dsrvLight, pLightTexSampler, LightTexSlot, LightTexBindFlag);
	//light texture제작

	//post processing layer
	m_pPostProcessingLayer = new CPostProcessingLayer(m_pCamera, m_pd3dDevice, m_pd3dDeviceContext);
	m_pPostProcessingLayer->Begin(m_RenderContainerSeller, pTexture);
	RECT CliensRect = m_pFrameWork->GetDisplaySize();
	//first pass data set
	m_pPostProcessingLayer->SetFirstPassData(CliensRect.right, CliensRect.bottom);
	m_pPostProcessingLayer->SetBloomThreshold(2.0f);
	float fMiddleGrey = 0.0025f;
	float fWhite = 1.5f;
	float fBloomScale = 0.1f;
	m_pPostProcessingLayer->SetFinalPassData(fMiddleGrey, fWhite, fBloomScale);
	//post processing layer
}

void CSceneMain::SetForwardRenderTargets() {
	ID3D11RenderTargetView *pd3dRTVs[RENDER_TARGET_NUMBER] = { m_pd3drtvColorSpecInt, m_pd3drtvNormal, m_pd3drtvSpecPow };
	float fClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	if (m_pd3drtvColorSpecInt) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvColorSpecInt, fClearColor);
	if (m_pd3drtvNormal) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
	if (m_pd3drtvSpecPow) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvSpecPow, fClearColor);

	m_pFrameWork->SetRenderTargetViews(RENDER_TARGET_NUMBER, pd3dRTVs);

	//multi thread render 코드
	/*
	m_pFrameWork->SetMainRenderTargetView();

	multi thread rendering 
	for (auto &pRenderThreadInfo : m_vRenderingThreadInfo)
	{
		pRenderThreadInfo.m_pd3dDeferredContext->OMSetRenderTargets(MULITE_RENDER_NUMBER, (ID3D11RenderTargetView **)&pd3dRTVs, m_pd3dDepthStencilView);
		::SetEvent(pRenderThreadInfo.m_hRenderingBeginEvent);
	}
	*/
}
void CSceneMain::ForwardRender() {

	m_pFrameWork->ClearDepthStencilView();

	//set 4 render target view 
	SetForwardRenderTargets();

	m_pSpaceContainer->PrepareRender(m_pCamera);

	m_pObjectLayer->Render();

	//ㅋㅋ 나중에 밖으로 빼자.. debuger init
	DEBUGER->DebugRender();
	//ㅋㅋ 나중에 밖으로 빼자.. debuger init

}
void CSceneMain::DeferredRender() {
	//랜더타겟 재 설정
	//clear depthStencilview를 잊지 말자
	m_pFrameWork->ClearDepthStencilView();

	//set light rtv
	m_pFrameWork->SetRenderTargetViews(1, &m_pd3drtvLight);
	//m_pFrameWork->SetMainRenderTargetView();

	//render light!
	m_pLightLayer->Render();

	//ㅋㅋ 나중에 밖으로 빼자.. debuger init
	//DEBUGER->RenderLightVolume();
	//ㅋㅋ 나중에 밖으로 빼자.. debuger init

}
void CSceneMain::PostProcessing() {
	//clear depthStencilview를 잊지 말자
	m_pFrameWork->ClearDepthStencilView();

	//진짜 rtv set!
	//m_pFrameWork->ClearRenderTargetView();
	m_pFrameWork->SetMainRenderTargetView();

	//rtv에 풀스크린 드로우 
	m_pPostProcessingLayer->Render();


	DEBUGER->AddTexture(XMFLOAT2(100, 100), XMFLOAT2(250, 250), m_pd3dsrvColorSpecInt);
	DEBUGER->AddTexture(XMFLOAT2(100, 250), XMFLOAT2(250, 400), m_pd3dsrvNormal);
	DEBUGER->AddTexture(XMFLOAT2(100, 400), XMFLOAT2(250, 550), m_pd3dsrvLight);
	//debuger clear 
	//DEBUGER->ClearDebuger();
}
void CSceneMain::ReleaseForwardRenderTargets() {
	if(m_pd3dtxtColorSpecInt) m_pd3dtxtColorSpecInt->Release();//0
	m_pd3dtxtColorSpecInt = nullptr;

	if(m_pd3dtxtNormal) m_pd3dtxtNormal->Release();//1
	m_pd3dtxtNormal = nullptr;

	if(m_pd3dtxtSpecPow) m_pd3dtxtSpecPow->Release();//2
	m_pd3dtxtSpecPow = nullptr;

	if(m_pd3dsrvColorSpecInt) m_pd3dsrvColorSpecInt->Release();//0
	m_pd3dsrvColorSpecInt = nullptr;

	if(m_pd3dsrvNormal) m_pd3dsrvNormal->Release();//1
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvSpecPow) m_pd3dsrvSpecPow->Release();//2
	m_pd3dsrvSpecPow = nullptr;

	if(m_pd3drtvColorSpecInt) m_pd3drtvColorSpecInt->Release();//0
	m_pd3drtvColorSpecInt = nullptr;

	if(m_pd3drtvNormal) m_pd3drtvNormal->Release();//1
	m_pd3drtvNormal = nullptr;

	if(m_pd3drtvSpecPow) m_pd3drtvSpecPow->Release();//2
	m_pd3drtvSpecPow = nullptr;

	if (m_pd3drtvLight) m_pd3drtvLight->Release();
	m_pd3drtvLight = nullptr;
}

void CSceneMain::Render() {
	ForwardRender();

	DeferredRender();
	PostProcessing();

	
}

void CSceneMain::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//마우스 캡쳐를 하고 현재 마우스 위치를 가져온다.
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		//마우스 캡쳐를 해제한다.
		ReleaseCapture();
		break;

	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}
void CSceneMain::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {

	switch (nMessageID)
	{
	case WM_KEYUP:
		
		switch (wParam)
		{
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CSceneMain::ProcessInput(float fTimeElapsed) {
	bool bProcessedByScene = false;
	static UCHAR pKeyBuffer[256];
	float cxDelta = 0.0f, cyDelta = 0.0f;

	//if (m_pScene) bProcessedByScene = m_pScene->ProcessInput();
	if (!bProcessedByScene)
	{
		POINT ptCursorPos;
		//마우스를 캡쳐했으면 마우스가 얼마만큼 이동하였는 가를 계산한다.
		if (GetCapture() == m_hWnd)
		{
			//마우스 커서를 화면에서 없앤다(보이지 않게 한다).
			SetCursor(NULL);
			//현재 마우스 커서의 위치를 가져온다.
			GetCursorPos(&ptCursorPos);
			//마우스 버튼이 눌린 채로 이전 위치에서 현재 마우스 커서의 위치까지 움직인 양을 구한다.
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}

		//키입력 또는 마우스 움직임이 있었다면 
		if (GetKeyboardState(pKeyBuffer) || cxDelta || cyDelta)
		{
			m_pCamera->ProcessInput(fTimeElapsed, pKeyBuffer, cxDelta, cyDelta);
			//-----------------------------------player--------------------------------------
			//m_pPlayer->ProcessInput(fTimeElapsed, pKeyBuffer, cxDelta, cyDelta);
			//-----------------------------------player--------------------------------------
		}

	}

	
}

CSceneMain::CSceneMain(CDirectXFramework* pFrameWork) : CScene("Main") {
	m_pFrameWork = pFrameWork;
	m_pd3dDevice = m_pFrameWork->GetDevice();
	m_pd3dDeviceContext = m_pFrameWork->GetDeviceContext();
	m_hWnd = m_pFrameWork->GethWnd();

	m_pd3dtxtColorSpecInt = nullptr;
	m_pd3dtxtNormal = nullptr;
	m_pd3dtxtSpecPow = nullptr;

	m_pd3dsrvColorSpecInt = nullptr;
	m_pd3dsrvNormal = nullptr;
	m_pd3dsrvSpecPow = nullptr;

	m_pd3drtvColorSpecInt= nullptr;
	m_pd3drtvNormal = nullptr;
	m_pd3drtvSpecPow = nullptr;

}
CSceneMain::~CSceneMain() {

}


