#include "stdafx.h"
#include "InstancingShader.h"

//create shader
bool CInstancingShader::CreateVS() {
	// 번역된 hlsl인 cso 로딩 후 VS와 IL 생성 : 추후 멀티스레드로 구현을 해야 할 부분
	auto loadVS = ReadCSOFile(TEXT("VSInctancing.cso"));
	{
		m_pd3dDevice->CreateVertexShader(
			&(loadVS[0])
			, loadVS.size()
			, nullptr
			, &m_vertexShader
			);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//model
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//color
			{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};

		m_pd3dDevice->CreateInputLayout(
			vertexDesc
			, static_cast<UINT>(GetArraySize(vertexDesc))
			, &(loadVS[0])
			, loadVS.size()
			, &m_inputLayout
			);
	}

	if (m_vertexShader) return true;

	return false;

}
//bool CTestShader::CreateGS() {
//
//	return true;
//}
//bool CTestShader::CreateHS() {
//
//	return true;
//}
//bool CTestShader::CreateCS() {
//
//	return true;
//}
bool CInstancingShader::CreatePS() {

	// 번역된 hlsl인 cso 로딩 후 PS 생성 : 추후 멀티스레드로 구현을 해야 할 부분
	auto loadPS = ReadCSOFile(TEXT("PSInstancing.cso"));
	{
		m_pd3dDevice->CreatePixelShader(
			&(loadPS[0])
			, loadPS.size()
			, nullptr
			, &m_pixelShader
			);

	}

	if (m_pixelShader) return true;

	return false;
}
//create shader


CInstancingShader::CInstancingShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderShader(pd3dDevice, pd3dDeviceContext) {

}
CInstancingShader::~CInstancingShader() {

}