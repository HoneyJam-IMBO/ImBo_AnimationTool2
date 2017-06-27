#include "stdafx.h"
#include "DirectXFramework.h"

void CDirectXFramework::Begin(HINSTANCE hInstance, HWND hWnd)
{

	m_hInstance = hInstance;
	m_hWnd = hWnd;
	GetClientRect(m_hWnd, &m_rcClient);

	_tcscpy_s(m_pszBuffer, _T("DXMAIN ("));
	
	if (false == CreateD3D11Deivce()) return;

	//device ���� ���� �ؾ���
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
	//device ���� ���� �ؾ���

	// render target�� depth-stencil buffer ����/ deferred texture ����
	if (!CreateRenderTargetView()) {
		MessageBox(m_hWnd, TEXT("RenderTarget�̳� Depth-Stencil ���� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return;
	}

	//camera
	m_pCamera = make_shared<CFreeCamera>(m_pd3dDevice, m_pd3dDeviceContext);
	//------------------------------------------ī�޶� ����--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(
		// FOV Y �� : Ŭ ���� �ָ����� �� �� �ִ�.
		60.0f * XM_PI / 180.0f
		// ��Ⱦ��
		, float(m_rcClient.right) / float(m_rcClient.bottom)
		// �ּ� �Ÿ�
		, 0.01f
		// �ִ� �Ÿ�
		, 10000.0f);

	//��ġ ����
	//viewprojection��� ����
	XMVECTOR eye = { 0.0f, 50.0f, -50.0f, 0.0f };
	XMVECTOR at = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_pCamera->SetLookAt(eye, at, up);
	// RS�� ����Ʈ ����
	m_pCamera->SetViewport(0, 0, m_rcClient.right, m_rcClient.bottom, 0.0f, 1.0f);
	//------------------------------------------ī�޶� ����--------------------------------------


	//TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	//int barSize[2] = { 224, 320 };
	//TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);
	
}

void CDirectXFramework::End() {
	// Release ��ü
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();

	// Rendering �߰� �κ� : Depth-Stencil �� ���� ������ �������� ť�갡 ��µ��� �ʴ´�.
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

	//����̽��� �����ϱ� ���Ͽ� �õ��� ����̹� ������ ������ ��Ÿ����.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//����̽��� �����ϱ� ���Ͽ� �õ��� Ư�� ������ ������ ��Ÿ����.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//������ ���� ü���� �����ϴ� ����ü�̴�.
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
	//����̽��� ����̹� ������ Ư�� ������ �����ϴ� ����̽��� ���� ü���� �����Ѵ�. �������� ����̽� ������ �õ��ϰ� �����ϸ� ���� ������ ����̽��� �����Ѵ�.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pdxgiSwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pdxgiSwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	return true;
}

bool CDirectXFramework::CreateRenderTargetView(){

	// Result Handle �Դϴ�. ��ġ�� ���������� ���������� �˻��մϴ�.
	HRESULT hResult = S_OK;

	// render target �� �����ϱ� ���� back buffer �� SwapChain ���� ��û�մϴ�.
	ID3D11Texture2D *pd3dBackBuffer{ nullptr };

	if (FAILED(hResult = m_pdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);

	// ��ȯ���� ���۸� ����Ͽ� render target view �� �����մϴ�.
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);

	// back buffer �� ��ȯ�մϴ�.
	if (pd3dBackBuffer) pd3dBackBuffer->Release();


	// Rendering �߰� �κ� : Depth-Stencil �� ���� ������ �������� ť�갡 ��µ��� �ʴ´�.
	{
		// depth stencil "texture" �� �����մϴ�.
		D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;

		// �޸𸮴� 0���� �ʱ�ȭ�մϴ�.
		ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));



		// Width : texture �� �ʺ��Դϴ�.
		d3dDepthStencilBufferDesc.Width = m_rcClient.right;
		// Height : texture �� �����Դϴ�.
		d3dDepthStencilBufferDesc.Height = m_rcClient.bottom;
		if (m_rcClient.right == 0 & m_rcClient.bottom == 0) {
			// Width : texture �� �ʺ��Դϴ�.
			d3dDepthStencilBufferDesc.Width += 1;
			// Height : texture �� �����Դϴ�.
			d3dDepthStencilBufferDesc.Height += 1;
		}
		// MipLevels : texture �ִ� MipMap Level ��. 
		//				���� ���ø� �ؽ�ó : 1
		//				�ִ� �Ӹ� ���� : 0
		d3dDepthStencilBufferDesc.MipLevels = 1;
		// ArraySize :texture �迭�� texture ����. (�迭�� �ƴϸ� 1)
		d3dDepthStencilBufferDesc.ArraySize = 1;
		// Format : texture �ȼ� ����
		d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// SampleDesc : ���� ���ø��� ǰ���� ����
		// CheckMultisampleQualityLevels �Լ��� ����Ͽ� ���� ���ø� ���� ���θ� Ȯ���� �ڿ� �� ����
		{

			// Count : �ȼ� �� ���� ����
			//	1  : ���� ���ø��� ���� ����
			//	2~ : �ش� ����ŭ�� ���� ���ø�
			d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
			// Quality : ǰ�� ����
			// 0 : ���� ���ø��� ���� ����
			d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
		}

		// Usage : texture �� �а� ���� ����� ���� ����
		d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		// BindFlags : ���������� �ܰ� ��� ������ ������ ����
		d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		// MiscFlags : ���ҽ��� ���� �߰� ���� ����. ������� ������ 0.
		d3dDepthStencilBufferDesc.MiscFlags = 0;
		// CPUAccessFlags : CPU�� ���۸� ����� �� �ִ� ����. ������� ������ 0.
		d3dDepthStencilBufferDesc.CPUAccessFlags = 0;

		//	�⺻ ����-���ٽ� ������ ���� ���۸� �������� �Ѵ�.
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

		//�ڱ� texture set -> sampler set����

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


		//light texture����
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
		//light texture����

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


	// Direct2D : RenderTarget���� 2DBackBuffer�� ȹ���մϴ�.
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
		//�̰� �� ���⼭ �������.

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
	//postprocessinglayer ������ set
	m_pPostProcessingLayer->SetAdaptation(fTimeElapsed);

	m_pCamera->Update(fTimeElapsed);
	ProcessInput(fTimeElapsed);

	//-----------------------------���� �� ����--------------------------------------
	m_stackScene.top()->Animate(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------

	INPUTMGR->SetWheel(WHEEL_NON);

	if(INPUTMGR->KeyDown(VK_0) ){
		TWBARMGR->DeleteBar("TweakBarTest2");
	}
	
}

void CDirectXFramework::ProcessInput(float fTimeElapsed) {
	INPUTMGR->Update(fTimeElapsed);

	//-----------------------------���� �� ����--------------------------------------
	m_stackScene.top()->ProcessInput(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------
	
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
		/*�������� ũ�Ⱑ ����� ��(����� ��Alt+Enter�� ��ü ȭ�� ���� ������ ���� ��ȯ�� ��) ���� ü���� �ĸ���� ũ�⸦ �����ϰ� �ĸ���ۿ� ���� ���� Ÿ�� �並 �ٽ� �����Ѵ�. */
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
	//���� �ƿ� �ٲ� ������ ���� pop�ϰ� 
	if (m_nScene != 0) {
		//������ Scene�� �����ϸ�

		PopScene();
		//������ scene�� ������

		PushScene(pScene);
		//���ο� scene�� ���� ����
	}
	else {
		//�ƴϸ� ó�� �ִ� Scene�̸� �׳�

		PushScene(pScene);
		//���ο� scene�� ���� ����
	}
}
void CDirectXFramework::PopScene() {
	m_stackScene.top()->End();
	delete m_stackScene.top();

	m_stackScene.pop();
	//scene�� ������

	if (m_stackScene.top())	m_stackScene.top()->Begin();
	//top�� ������ begin
}
void CDirectXFramework::PushScene(CScene* pScene) {
	pScene->Begin();
	m_stackScene.push(pScene);
	//���ο� scene�� ���� ����
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

	//multi thread render �ڵ�
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
	// OM�� RenderTarget �缳��
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
}
void CDirectXFramework::SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs) {
	m_pd3dDeviceContext->OMSetRenderTargets(nRenderTarget, pd3dRTVs, m_pd3dDepthStencilView);
}
void CDirectXFramework::ObjectRender(){
	//set
	//-----------------------------ī�޶� ���� set------------------------------------
	m_pCamera->SetShaderState();
	//-----------------------------ī�޶� ���� set------------------------------------
	//-----------------------------���� �� ����--------------------------------------
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

	//��¥ rtv set!
	SetMainRenderTargetView();

	for (auto texture : m_vLightLayerResultTexture) {
		texture->SetShaderState();
	}
	//rtv�� Ǯ��ũ�� ��ο� 
	m_pPostProcessingLayer->Render(m_pCamera);

	for (auto texture : m_vLightLayerResultTexture) {
		texture->CleanShaderState();
	}
}
void CDirectXFramework::ClearDepthStencilView() {
	// Rendering �߰� �κ� : Depth-Stencil �� ���� ������ �������� ť�갡 ��µ��� �ʴ´�.
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