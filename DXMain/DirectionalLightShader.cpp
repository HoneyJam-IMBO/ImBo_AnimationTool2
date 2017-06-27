#include "stdafx.h"
#include "DirectionalLightShader.h"

bool CDirectionalLightShader::CreateVS() {
	auto loadVS = ReadCSOFile(TEXT("VSDirectionalLight.cso"));
	{
		m_pd3dDevice->CreateVertexShader(
			&(loadVS[0])
			, loadVS.size()
			, nullptr
			, &m_vertexShader
		);

	}

	if (m_vertexShader) return true;

	return false;


}
bool CDirectionalLightShader::CreatePS() {

	// 번역된 hlsl인 cso 로딩 후 PS 생성 : 추후 멀티스레드로 구현을 해야 할 부분
	auto loadPS = ReadCSOFile(TEXT("PSDirectionalLight.cso"));
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


CDirectionalLightShader::CDirectionalLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderShader(pd3dDevice, pd3dDeviceContext) {

}
CDirectionalLightShader::~CDirectionalLightShader() {

}