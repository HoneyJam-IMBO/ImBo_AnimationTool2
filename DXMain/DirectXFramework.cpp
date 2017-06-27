#include "stdafx.h"
#include "DirectXFramework.h"

void CDirectXFramework::Begin(HINSTANCE hInstance, HWND hWnd)
{

	m_hInstance = hInstance;
	m_hWnd = hWnd;
	GetClientRect(m_hWnd, &m_rcClient);

	_tcscpy_s(m_pszBuffer, _T("DXMAIN ("));
	
	if (false == CreateD3D11Deivce()) return;

	//device 생성 이후 해야함
	//singleton Init 
	RESOURCEMGR->Begin(m_pd3dDevice, m_pd3dDeviceContext);
	RCSELLER->Begin(m_pd3dDevice, m_pd3dDeviceContext);
	DEBUGER->Begin(m_pd3dDevice, m_pd3dDeviceContext);
	INPUTMGR->Begin(m_hWnd);
	TWBARMGR->Begin(m_pd3dDevice, m_pd3dDeviceContext, 
		" GLOBAL help='test ui death' ");
	DOTXIMPORTER->ReadyImport(m_hWnd, m_rcClient.right, m_rcClient.bottom);
	//DOTXIMPORTER->Begin(L"../../Assets/mesh/House01_B/House01_B.x");
	//singleton Init
	//post processing layer
	m_pPostProcessingLayer = new CPostProcessingLayer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pPostProcessingLayer->Begin();
	//post processing layer
	//device 생성 이후 해야함

	// render target과 depth-stencil buffer 생성/ deferred texture 생성
	if (!CreateRenderTargetView()) {
		MessageBox(m_hWnd, TEXT("RenderTarget이나 Depth-Stencil 버퍼 생성이 실패했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
		return;
	}

	//camera
	m_pCamera = make_shared<CFreeCamera>(m_pd3dDevice, m_pd3dDeviceContext);
	//------------------------------------------카메라 제작--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(
		// FOV Y 값 : 클 수록 멀리까지 볼 수 있다.
		60.0f * XM_PI / 180.0f
		// 종횡비
		, float(m_rcClient.right) / float(m_rcClient.bottom)
		// 최소 거리
		, 0.01f
		// 최대 거리
		, 10000.0f);

	//위치 조정
	//viewprojection행렬 제작
	XMVECTOR eye = { 0.0f, 50.0f, -50.0f, 0.0f };
	XMVECTOR at = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_pCamera->SetLookAt(eye, at, up);
	// RS에 뷰포트 연결
	m_pCamera->SetViewport(0, 0, m_rcClient.right, m_rcClient.bottom, 0.0f, 1.0f);
	//------------------------------------------카메라 제작--------------------------------------


	//TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	//int barSize[2] = { 224, 320 };
	//TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);
	
}

void CDirectXFramework::End() {
	// Release 객체
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();

	// Rendering 추가 부분 : Depth-Stencil 을 하지 않으면 정상적인 큐브가 출력되지 않는다.
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

	//camera end
	if (m_pCamera){
		m_pCamera->End();
	}

	//scene end
	for (int i = 0; i < m_nScene; ++i) {
		m_stackScene.top()->End();
		m_stackScene.pop();
	}
	
	ReleaseForwardRenderTargets();

	//layer
	if (m_pPostProcessingLayer) {
		m_pPostProcessingLayer->End();
		delete m_pPostProcessingLayer;
	}

	//singleton End
	RESOURCEMGR->End();
	RCSELLER->End();
	DEBUGER->End();
	INPUTMGR->End();
	//singleton End

//	//ui
	TWBARMGR->End();
}

bool CDirectXFramework::CreateD3D11Deivce()
{
	UINT dwCreateDeviceFlags = 0;
#ifdef _DEBUG
	dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//디바이스를 생성하기 위하여 시도할 드라이버 유형의 순서를 나타낸다.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//디바이스를 생성하기 위하여 시도할 특성 레벨의 순서를 나타낸다.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//생성할 스왑 체인을 서술하는 구조체이다.
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_rcClient.right;
	dxgiSwapChainDesc.BufferDesc.Height = m_rcClient.bottom;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;

	D3D_DRIVER_TYPE nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;
	//디바이스의 드라이버 유형과 특성 레벨을 지원하는 디바이스와 스왑 체인을 생성한다. 고수준의 디바이스 생성을 시도하고 실패하면 다음 수준의 디바이스를 생성한다.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pdxgiSwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pdxgiSwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	return true;
}

bool CDirectXFramework::CreateRenderTargetView(){

	// Result Handle 입니다. 장치가 성공적으로 생성도는지 검사합니다.
	HRESULT hResult = S_OK;

	// render target 을 생성하기 위한 back buffer 를 SwapChain 에게 요청합니다.
	ID3D11Texture2D *pd3dBackBuffer{ nullptr };

	if (FAILED(hResult = m_pdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);

	// 반환받은 버퍼를 사용하여 render target view 를 생성합니다.
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);

	// back buffer 를 반환합니다.
	if (pd3dBackBuffer) pd3dBackBuffer->Release();


	// Rendering 추가 부분 : Depth-Stencil 을 하지 않으면 정상적인 큐브가 출력되지 않는다.
	{
		// depth stencil "texture" 를 생성합니다.
		D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;

		// 메모리는 0으로 초기화합니다.
		ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));



		// Width : texture 의 너비입니다.
		d3dDepthStencilBufferDesc.Width = m_rcClient.right;
		// Height : texture 의 높이입니다.
		d3dDepthStencilBufferDesc.Height = m_rcClient.bottom;
		if (m_rcClient.right == 0 & m_rcClient.bottom == 0) {
			// Width : texture 의 너비입니다.
			d3dDepthStencilBufferDesc.Width += 1;
			// Height : texture 의 높이입니다.
			d3dDepthStencilBufferDesc.Height += 1;
		}
		// MipLevels : texture 최대 MipMap Level 수. 
		//				다중 샘플링 텍스처 : 1
		//				최대 밉맵 레벨 : 0
		d3dDepthStencilBufferDesc.MipLevels = 1;
		// ArraySize :texture 배열의 texture 개수. (배열이 아니면 1)
		d3dDepthStencilBufferDesc.ArraySize = 1;
		// Format : texture 픽셀 형식
		d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// SampleDesc : 다중 샘플링의 품질을 지정
		// CheckMultisampleQualityLevels 함수를 사용하여 다중 샘플링 가능 여부를 확인한 뒤에 값 변경
		{

			// Count : 픽셀 당 샘플 개수
			//	1  : 다중 샘플링을 하지 않음
			//	2~ : 해당 수만큼의 다중 샘플링
			d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
			// Quality : 품질 레벨
			// 0 : 다중 샘플링을 하지 않음
			d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
		}

		// Usage : texture 를 읽고 쓰는 방법에 대한 정의
		d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		// BindFlags : 파이프라인 단계 어디에 연결할 것인지 정의
		d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		// MiscFlags : 리소스에 대한 추가 선택 사항. 사용하지 않으면 0.
		d3dDepthStencilBufferDesc.MiscFlags = 0;
		// CPUAccessFlags : CPU가 버퍼를 사용할 수 있는 유형. 사용하지 않으면 0.
		d3dDepthStencilBufferDesc.CPUAccessFlags = 0;

		//	기본 깊이-스텐실 버퍼의 깊이 버퍼를 읽으려고 한다.
		if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);

		// Create the depth stencil view 
		D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
		ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;

		d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

		if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);


	}
	{//create framework texture
		ReleaseForwardRenderTargets();
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
		d3dTexture2DDesc.Width = m_rcClient.right;
		d3dTexture2DDesc.Height = m_rcClient.bottom;
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
		d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

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
		ID3D11ShaderResourceView *pd3dSRV = { m_pd3dsrvColorSpecInt };
		UINT Slot = { 0 };
		UINT BindFlag = { BIND_PS };
		//make sampler
		shared_ptr<CSampler> pSampler = make_shared<CSampler>(m_pd3dDevice, m_pd3dDeviceContext);
		pSampler->Begin();
		shared_ptr<CTexture> pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvNormal };
		Slot = { 1 };
		BindFlag = { BIND_PS };
		pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvSpecPow };
		Slot = { 2 };
		BindFlag = { BIND_PS };
		pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);
		//---------------------make texture---------------------


		//light texture제작
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);

		pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
		//make texture
		UINT LightTexSlot = { 0 };
		UINT LightTexBindFlag = { BIND_PS | BIND_CS };
		//make sampler
		shared_ptr<CSampler> pLightTexSampler = make_shared<CSampler>(m_pd3dDevice, m_pd3dDeviceContext);
		UINT LightTexSamplerBindFlag = { BIND_PS | BIND_CS };
		UINT LightTexSamplerSlot = { 0 };
		pLightTexSampler->Begin(LightTexSamplerSlot, LightTexSamplerBindFlag);
		pTexture->Begin(m_pd3dsrvLight, pLightTexSampler, LightTexSlot, LightTexBindFlag);
		m_vLightLayerResultTexture.push_back(pTexture);
		//light texture제작

		//first pass data set
		m_pPostProcessingLayer->SetFirstPassData(m_rcClient.right, m_rcClient.bottom);
		m_pPostProcessingLayer->SetBloomThreshold(2.0f);
		float fMiddleGrey = 0.0025f;
		float fWhite = 1.5f;
		float fBloomScale = 0.1f;
		m_pPostProcessingLayer->SetFinalPassData(fMiddleGrey, fWhite, fBloomScale);
		//post processing layer
	}

	//	DXGI_FRAME_STATISTICS p;
	//	m_pdxgiSwapChain->GetFrameStatistics(&p);


	// Direct2D : RenderTarget에서 2DBackBuffer를 획득합니다.
	//return(CreateD2DBackBuffer());

	return true;
}

void CDirectXFramework::FrameAdvance()
{
	TIMEMGR->Tick();

	Update(TIMEMGR->GetTimeElapsed());
	//Update2D();

	Render();
	//Render2D();
	
	m_pdxgiSwapChain->Present(0, 0);

	TIMEMGR->GetFrameRate(m_pszBuffer + 8, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}


void CDirectXFramework::Render(){
	//scene
	ObjectRender();
	LightRender();
	//scene
	
	PostProcessing();


	if (INPUTMGR->GetDebugMode()) {
	//if(testBotton){
		//DEBUGER->AddTexture(XMFLOAT2(100, 100), XMFLOAT2(250, 250), m_pd3dsrvColorSpecInt);
		DEBUGER->AddTexture(XMFLOAT2(500, 0), XMFLOAT2(750, 150), m_pd3dsrvNormal);
		//DEBUGER->AddTexture(XMFLOAT2(100, 400), XMFLOAT2(250, 550), m_pd3dsrvLight);
		//DEBUGER->AddTexture(XMFLOAT2(250, 100), XMFLOAT2(500, 350), m_pd3dsrvSpecPow);
		//이건 꼭 여기서 해줘야함.

		DEBUGER->RenderTexture();
		DEBUGER->RenderText();
		//DEBUGER->ClearDebuger();
	}
	else {
		DEBUGER->ClearDebuger();
	}
	
//	//ui
	TWBARMGR->Render();
}

void CDirectXFramework::Update(float fTimeElapsed){
	//postprocessinglayer 적응값 set
	m_pPostProcessingLayer->SetAdaptation(fTimeElapsed);

	m_pCamera->Update(fTimeElapsed);
	ProcessInput(fTimeElapsed);

	//-----------------------------현재 씬 실행--------------------------------------
	m_stackScene.top()->Animate(fTimeElapsed);
	//-----------------------------현재 씬 실행--------------------------------------

	INPUTMGR->SetWheel(WHEEL_NON);

	if(INPUTMGR->KeyDown(VK_0) ){
		TWBARMGR->DeleteBar("TweakBarTest2");
	}
	
}

void CDirectXFramework::ProcessInput(float fTimeElapsed) {
	INPUTMGR->Update(fTimeElapsed);

	//-----------------------------현재 씬 실행--------------------------------------
	m_stackScene.top()->ProcessInput(fTimeElapsed);
	//-----------------------------현재 씬 실행--------------------------------------
	
}
void CDirectXFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	m_stackScene.top()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	
	switch (nMessageID)
	{

	case WM_LBUTTONDOWN:
		INPUTMGR->SetbCapture(true);
		INPUTMGR->SetOldCursorPos();
		INPUTMGR->SetMouseLeft(true);
		break;

	case WM_RBUTTONDOWN:
		INPUTMGR->SetbCapture(true);
		INPUTMGR->SetOldCursorPos();
		INPUTMGR->SetMouseRight(true);
		break;

	case WM_LBUTTONUP:
	{
		//static bool showCusor = true;
		//showCusor = showCusor ? false : true;
		//ShowCursor(showCusor);
		INPUTMGR->SetMouseLeft(false);
		INPUTMGR->SetbCapture(false);
		break;
	}

	case WM_RBUTTONUP:
		INPUTMGR->SetMouseRight(false);
		INPUTMGR->SetbCapture(false);
		break;
	
	case WM_MOUSEMOVE:
		INPUTMGR->SetMousePoint((int)LOWORD(lParam), (int)HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		((short)HIWORD(wParam) < WHEEL_NON) ?
			INPUTMGR->SetWheel(WHEEL_DOWN) :
			INPUTMGR->SetWheel(WHEEL_UP);
		break;
		//for drag&drop
	case WM_DROPFILES:
		INPUTMGR->ProcDropFile(wParam);
		
		break;
	default:
		break;
	}
}


void CDirectXFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam){
	m_stackScene.top()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		}
	}
}

LRESULT CALLBACK CDirectXFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	//	//ui
	if (TWBARMGR->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam)) return 0;

	switch (nMessageID)
	{
		/*윈도우의 크기가 변경될 때(현재는 “Alt+Enter“ 전체 화면 모드와 윈도우 모드로 전환될 때) 스왑 체인의 후면버퍼 크기를 조정하고 후면버퍼에 대한 렌더 타겟 뷰를 다시 생성한다. */
	case WM_SIZE:{
		m_rcClient.right = LOWORD(lParam);
		m_rcClient.bottom = HIWORD(lParam);

		DEBUGER->ResizeDisplay(m_rcClient);

		m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		if(FAILED(m_pdxgiSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
			MessageBox(nullptr, TEXT(""), TEXT(""), MB_OK);

		//resize rtv size, deferred texture size
		CreateRenderTargetView();

		m_pCamera->SetViewport(0, 0, m_rcClient.right, m_rcClient.bottom, 0.0f, 1.0f);
		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_DROPFILES:
		INPUTMGR->ProcDropFile(wParam);

		break;
	}
	return(0);
}

//--------------------------------------scene-----------------------------------------
void CDirectXFramework::ChangeScene(CScene* pScene) {
	//씬을 아예 바꿈 이전의 씬을 pop하고 
	if (m_nScene != 0) {
		//기존의 Scene이 존재하면

		PopScene();
		//기존의 scene을 빼내고

		PushScene(pScene);
		//새로운 scene을 집어 넣음
	}
	else {
		//아니면 처음 넣는 Scene이면 그냥

		PushScene(pScene);
		//새로운 scene을 집어 넣음
	}
}
void CDirectXFramework::PopScene() {
	m_stackScene.top()->End();
	delete m_stackScene.top();

	m_stackScene.pop();
	//scene을 빼내고

	if (m_stackScene.top())	m_stackScene.top()->Begin();
	//top이 있으면 begin
}
void CDirectXFramework::PushScene(CScene* pScene) {
	pScene->Begin();
	m_stackScene.push(pScene);
	//새로운 scene을 집어 넣음
	++m_nScene;
}
//--------------------------------------scene-----------------------------------------
void CDirectXFramework::SetForwardRenderTargets() {
	ID3D11RenderTargetView *pd3dRTVs[RENDER_TARGET_NUMBER] = { m_pd3drtvColorSpecInt, m_pd3drtvNormal, m_pd3drtvSpecPow };
	//float fClearColor[4] = { xmf4Xolor.x, xmf4Xolor.y, xmf4Xolor.z, xmf4Xolor.w };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 0.f };
	if (m_pd3drtvColorSpecInt) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvColorSpecInt, fClearColor);
	if (m_pd3drtvNormal) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
	if (m_pd3drtvSpecPow) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvSpecPow, fClearColor);

	SetRenderTargetViews(RENDER_TARGET_NUMBER, pd3dRTVs);

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


//--------------------------------------deferred rendering func-----------------------------
void CDirectXFramework::SetMainRenderTargetView() {
	// OM에 RenderTarget 재설정
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
}
void CDirectXFramework::SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs) {
	m_pd3dDeviceContext->OMSetRenderTargets(nRenderTarget, pd3dRTVs, m_pd3dDepthStencilView);
}
void CDirectXFramework::ObjectRender(){
	//set
	//-----------------------------카메라 버퍼 set------------------------------------
	m_pCamera->SetShaderState();
	//-----------------------------카메라 버퍼 set------------------------------------
	//-----------------------------현재 씬 실행--------------------------------------
	ClearDepthStencilView();
	SetForwardRenderTargets();

	m_stackScene.top()->ObjectRender();

}
void CDirectXFramework::LightRender() {

	//light render
	ClearDepthStencilView();
	//set light rtv
	SetRenderTargetViews(1, &m_pd3drtvLight);
	//m_pFrameWork->SetMainRenderTargetView();
	for (auto texture : m_vObjectLayerResultTexture) {
		texture->SetShaderState();
	}
	m_stackScene.top()->LightRender();
	for (auto texture : m_vObjectLayerResultTexture) {
		texture->CleanShaderState();
	}
	//light render
}
void CDirectXFramework::PostProcessing() {
	//postprocessing
	//clear depthStencilview
	ClearDepthStencilView();

	//진짜 rtv set!
	SetMainRenderTargetView();

	for (auto texture : m_vLightLayerResultTexture) {
		texture->SetShaderState();
	}
	//rtv에 풀스크린 드로우 
	m_pPostProcessingLayer->Render(m_pCamera);

	for (auto texture : m_vLightLayerResultTexture) {
		texture->CleanShaderState();
	}
}
void CDirectXFramework::ClearDepthStencilView() {
	// Rendering 추가 부분 : Depth-Stencil 을 하지 않으면 정상적인 큐브가 출력되지 않는다.
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void CDirectXFramework::ClearRenderTargetView() {
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, FLOAT4{ 0.0f, 0.5f, 0.8f, 1.0f });
}

void CDirectXFramework::ReleaseForwardRenderTargets() {
	//texture end
	m_vObjectLayerResultTexture.clear();
	m_vLightLayerResultTexture.clear();

	if (m_pd3dtxtColorSpecInt) m_pd3dtxtColorSpecInt->Release();//0
	m_pd3dtxtColorSpecInt = nullptr;

	if (m_pd3dtxtNormal) m_pd3dtxtNormal->Release();//1
	m_pd3dtxtNormal = nullptr;

	if (m_pd3dtxtSpecPow) m_pd3dtxtSpecPow->Release();//2
	m_pd3dtxtSpecPow = nullptr;

	if (m_pd3dsrvColorSpecInt) m_pd3dsrvColorSpecInt->Release();//0
	m_pd3dsrvColorSpecInt = nullptr;

	if (m_pd3dsrvNormal) m_pd3dsrvNormal->Release();//1
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvSpecPow) m_pd3dsrvSpecPow->Release();//2
	m_pd3dsrvSpecPow = nullptr;

	if (m_pd3drtvColorSpecInt) m_pd3drtvColorSpecInt->Release();//0
	m_pd3drtvColorSpecInt = nullptr;

	if (m_pd3drtvNormal) m_pd3drtvNormal->Release();//1
	m_pd3drtvNormal = nullptr;

	if (m_pd3drtvSpecPow) m_pd3drtvSpecPow->Release();//2
	m_pd3drtvSpecPow = nullptr;

	if (m_pd3drtvLight) m_pd3drtvLight->Release();
	m_pd3drtvLight = nullptr;

}
//--------------------------------------deferred rendering func-----------------------------

CDirectXFramework::~CDirectXFramework(){

}
