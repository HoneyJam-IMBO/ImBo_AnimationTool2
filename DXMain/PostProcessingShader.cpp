#include "stdafx.h"
#include "PostProcessingShader.h"

/*
struct AMBIENT_LIGHT {
XMFLOAT4 m_AmbientDown;
XMFLOAT4 m_AmbientRange;
XMFLOAT4 m_AmbientUp;
XMFLOAT4 m_AmbientColor;
};
*/
//create shader
bool CPostProcessingShader::CreateVS() {
	auto loadVS = ReadCSOFile(TEXT("VSPostProcessing.cso"));
	{
		m_pd3dDevice->CreateVertexShader(
			&(loadVS[0])
			, loadVS.size()
			, nullptr
			, &m_vertexShader
		);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
bool CPostProcessingShader::CreatePS() {

	// 번역된 hlsl인 cso 로딩 후 PS 생성 : 추후 멀티스레드로 구현을 해야 할 부분
	auto loadPS = ReadCSOFile(TEXT("PSPostProcessing.cso"));
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


CPostProcessingShader::CPostProcessingShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderShader(pd3dDevice, pd3dDeviceContext) {

}
CPostProcessingShader::~CPostProcessingShader() {

}