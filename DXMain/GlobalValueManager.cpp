
#include "stdafx.h"
#include "GlobalValueManager.h"

bool CGlobalValueManager::Begin(HINSTANCE hInst, HWND hWnd) {
	SethInst(hInst);
	SethWnd(hWnd);
	GetClientRect(m_hWnd, &m_rcClient);

	//create swapchain, device, device context
	{
		// Swap Chain Description 구조체
		DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
#pragma region [DXGI_SWAP_CHAIN_DESC 초기화]
		// 구조체 비우기
		::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
		// BufferCount : 후면 버퍼의 수를 지정
		dxgiSwapChainDesc.BufferCount = 2;

		// BufferDesc : 후면 버퍼의 디스플레이 형식을 지정
		{
			// Width : 버퍼의 가로 크기를 픽셀로 지정
			dxgiSwapChainDesc.BufferDesc.Width = m_rcClient.right;
			// Height : 버퍼의 세로 크기를 픽셀로 지정
			dxgiSwapChainDesc.BufferDesc.Height = m_rcClient.bottom;
			// Format : 후면 버퍼 픽셀 형식
			/// DirectX 11-1(Chap 01)-Device, p.49 참조
			dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			// RefreshRate : 화면 갱신 비율을 Hz 단위로 지정
			{
				// Denominator : 분모
				dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
				// Numerator : 분자
				dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			}
			// ScanlineOrdering : scan line 그리기 모드 지정(기본 0)
			//	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED        (0) : 스캔 라인 순서를 지정하지 않음	
			//	DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE        (1) : 프로그레시브 스캔 라인 순서 지정
			//	DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST  (2) : 상위 필드로 이미지 생성
			//	DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST  (3) : 하위 필드로 이미지 생성
			dxgiSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		}

		// BufferUsage : 후면 버퍼에 대한 표면 사용 방식과 CPU의 접근 방법 지정
		//	DXGI_USAGE_SHADER_INPUT				: shader 의 입력으로 사용
		//	DXGI_USAGE_RENDER_TARGET_OUTPUT		: render target으로 사용
		//	DXGI_USAGE_BACK_BUFFER         		: back buffer 로 사용
		//	DXGI_USAGE_SHARED              		: 공유 목적
		//	DXGI_USAGE_READ_ONLY           		: 읽기 전용
		//	DXGI_USAGE_DISCARD_ON_PRESENT  		: DXGI 내부 전용 사용(사용자가 사용하지 않음)
		//	DXGI_USAGE_UNORDERED_ACCESS    		: 무순서화 접근
		dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// OutputWindow : 출력 윈도우의 핸들을 지정(반드시 유효해야 함)
		dxgiSwapChainDesc.OutputWindow = m_hWnd;

		// SampleDesc : 다중 샘플링의 품질을 지정
		// CheckMultisampleQualityLevels 함수를 사용하여 다중 샘플링 가능 여부를 확인한 뒤에 값 변경
		{
			// Count : 픽셀 당 샘플 개수
			//	1  : 다중 샘플링을 하지 않음
			//	2~ : 해당 수만큼의 다중 샘플링
			dxgiSwapChainDesc.SampleDesc.Count = 1;
			// Quality : 품질 레벨
			// 0 : 다중 샘플링을 하지 않음
			dxgiSwapChainDesc.SampleDesc.Quality = 0;
		}

		// Windowed : 윈도우 모드 또는 전체 화면 모드 지정 ~ TRUE  | 창 모드 
		//												   ~ FALSE | 전체 화면
		dxgiSwapChainDesc.Windowed = TRUE;

		// Flags : Swap Chain 동작에 대한 선택 사항을 지정
		//	DXGI_SWAP_CHAIN_FLAG_NONPREROTATED		(1) : 전체 화면 모드에서 전면 버퍼의 내용을 화면으로 옮길 때 자동 회전하지 않음
		//	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH	(2) : 응용 프로그램이 디스플레이 모드를 변경할 수 있음
		//	DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE		(4) : 응용 프로그램이 GDI 로 랜더링 할 수 있음. 후면 버퍼에 GetDC() 사용 가능
		dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH/*2*/;

		// SwapEffect : Swaping을 처리하는 선택사항을 지정(기본 : 0)
		//	DXGI_SWAP_EFFECT_DISCARD		(0) : 버퍼 내용을 폐기
		//	DXGI_SWAP_EFFECT_SEQUENTIAL		(1) : 순차 복사
		// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL	(2) : Flip 순차 복사
		dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

#pragma endregion


		// 지원하는 하드웨어 그래픽 드라이버를 열거합니다.
		D3D_DRIVER_TYPE d3dDriverTypes[]
		{
			// 하드웨어 드라이버, Direct3D의 기능이 최대한 하드웨어로 구현 
			// 하드웨어적으로 제공하지 않는 기능은 소프트웨어를 통해 구현
			D3D_DRIVER_TYPE_HARDWARE
			// DirectX SDK에서 제공하는 고성능의 소프트웨어 드라이버
			// 특성 레벨 9_1에서 특성 레벨 10.1까지 제공
			// - 별도의 소프트웨어 드라이버(래스터라이저)가 필요없음
			// - 소프트웨어 드라이버이면서 그래픽 하드웨어의 최대 성능을 지원
			//   Direct3D 11을 지원하지 않는 그래픽 하드웨어에서도 사용
			, D3D_DRIVER_TYPE_WARP
			// 참조 드라이버(모든 Direct3D 특성을 지원하는 소프트웨어 드라이버)
			// DirectX SDK에서 속도보다는 정확성을 위해 제공
			// CPU를 사용하여 래스터라이저 구현 
			// Direct3D 11의 모든 특성 레벨에서 이 드라이버를 사용할 수 있음
			// 응용 프로그램의 테스트와 디버깅을 위해 주로 사용
			, D3D_DRIVER_TYPE_REFERENCE
		};

		// 이 배열은 이 응용 프로그램에서 지원하는 DirectX 하드웨어 기능 수준 집합을 정의합니다.
		// 순서를 유지해야 합니다.
		// 설명에서 응용 프로그램에 필요한 최소 기능 수준을 선언해야 합니다.
		// 별도로 지정하지 않은 경우 모든 응용 프로그램은 9.1을 지원하는 것으로 간주됩니다.
		D3D_FEATURE_LEVEL d3dFeatureLevels[]
		{
			D3D_FEATURE_LEVEL_11_1
			, D3D_FEATURE_LEVEL_11_0
			, D3D_FEATURE_LEVEL_10_1
			, D3D_FEATURE_LEVEL_10_0
		};
		// 지원 가능한 모든 하드웨어 기능 수준 집합 수를 구합니다.
		auto szFeatureLevel = static_cast<UINT>(GetArraySize(d3dFeatureLevels));

		// 최종적으로 사용될 장치의 기능 수준을 저장하기 위한 변수입니다.
		D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_1;

		// Result Handle 입니다. 장치가 성공적으로 생성도는지 검사합니다.
		HRESULT hResult = S_OK;

		// 이 플래그는 API 기본값과 다른 색 채널 순서의 표면에 대한 지원을
		// 추가합니다. Direct2D와의 호환성을 위해 필요합니다.
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)

		auto IsSDKLayerAvailable = []() -> bool {
			return SUCCEEDED(D3D11CreateDevice(nullptr
				, D3D_DRIVER_TYPE_NULL			// 실제 하드웨어 장치를 만들 필요가 없습니다.
				, 0
				, D3D11_CREATE_DEVICE_DEBUG		// SDK 레이어를 확인하세요.
				, nullptr						// 모든 기능 수준이 적용됩니다.
				, 0
				, D3D11_SDK_VERSION				// Windows 스토어 앱의 경우 항상 이 값을 D3D11_SDK_VERSION으로 설정합니다.
				, nullptr						// D3D 장치 참조를 보관할 필요가 없습니다.
				, nullptr						// 기능 수준을 알 필요가 없습니다.
				, nullptr						// D3D 장치 컨텍스트 참조를 보관할 필요가 없습니다.
			));
		};

		// SDK 레이어 지원을 확인하세요.
		if (IsSDKLayerAvailable())
		{
			// 프로젝트가 디버그 빌드 중인 경우에는 이 플래그가 있는 SDK 레이어를 통해 디버깅을 사용하십시오.
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}
#endif

		for (D3D_DRIVER_TYPE &p : d3dDriverTypes)
		{
			if (SUCCEEDED(hResult = D3D11CreateDevice(NULL						// 기본 어댑터를 사용하려면 nullptr을 지정합니다.
				, p							// 하드웨어 그래픽 드라이버를 사용하여 장치를 만듭니다.
				, 0							// 드라이버가 D3D_DRIVER_TYPE_SOFTWARE가 아닌 경우 0이어야 합니다.
				, creationFlags				// 디버그 및 Direct2D 호환성 플래그를 설정합니다.
				, d3dFeatureLevels			// 이 응용 프로그램이 지원할 수 있는 기능 수준 목록입니다.
				, szFeatureLevel			// 위 목록의 크기입니다.
				, D3D11_SDK_VERSION			// Windows 스토어 앱의 경우 항상 이 값을 D3D11_SDK_VERSION으로 설정합니다.
				, &m_pd3dDevice				// 만들어진 Direct3D 장치를 반환합니다.
				, &nd3dFeatureLevel			// 만들어진 장치의 기능 수준을 반환합니다.
				, &m_pd3dDeviceContext		// 장치 직접 컨텍스트를 반환합니다.
			)
			)
				)
				break;
		}

		// 모든 지원 가능한 드라이버에서 생성이 실패하면 프로그램을 종료합니다.
		if (!m_pd3dDevice)
		{
			MessageBox(m_hWnd, TEXT("지원 가능한 그래픽 사양이 없습니다."), TEXT("프로그램 구동 실패"), MB_OK);
			return(false);
		}

		// DXGI Device 를 받습니다.
		//IDXGIDevice3 *pdxgiDevice = NULL;
		//if (FAILED(hResult = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice3), (LPVOID*)&pdxgiDevice)))
		IDXGIDevice *pdxgiDevice = NULL;
		if (FAILED(hResult = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&pdxgiDevice)))
		{
			MessageBox(m_hWnd, TEXT("DXGI Device 객체를 반환받지 못했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
			return(false);
		}
		// DXGI Factory 인스턴스를 DXGIFactory에서 받습니다.
		//IDXGIFactory *pdxgiFactory = NULL;
		IDXGIFactory *pdxgiFactory = NULL;

		UINT udxgiFlag = 0;
#ifdef _DEBUG
		udxgiFlag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		//if (FAILED(hResult = CreateDXGIFactory3(udxgiFlag, __uuidof(IDXGIFactory3), (LPVOID*)&pdxgiFactory)))
		if (FAILED(hResult = CreateDXGIFactory(/*udxgiFlag,*/ __uuidof(IDXGIFactory), (LPVOID*)&pdxgiFactory)))
		{
			MessageBox(m_hWnd, TEXT("DXGIFactory에서의 객체 생성이 실패했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
			return(false);
		}
		//  SwapChain 을 생성합니다
		if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pdxgiSwapChain)))
		{
			MessageBox(m_hWnd, TEXT("SwapChain 인스턴스 생성이 실패했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
			return(false);
		}

		//	// Direct2D : Direct2D 인스턴스를 생성합니다.
		//	if (!CreateD2D1Device(pdxgiDevice))
		//	{
		//		MessageBox(m_hWnd, TEXT("Direct2D 인스턴스 생성이 실패했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
		//		return(false);
		//	}

		// 할당받은 COM 객체를 반환합니다.
		if (pdxgiDevice) pdxgiDevice->Release();
		if (pdxgiFactory) pdxgiFactory->Release();

	}


	return true;
}

bool CGlobalValueManager::End() {
	// Release 객체
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();

	return true;
}

CGlobalValueManager::CGlobalValueManager() :CSingleTonBase<CGlobalValueManager>("globalvaluemanagersingleton") {

}

CGlobalValueManager::~CGlobalValueManager() {

}
