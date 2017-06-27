#include "stdafx.h"
#include "CapsuleLightShader.h"

bool CCapsuleLightShader::CreateVS() {
	auto loadVS = ReadCSOFile(TEXT("VSCapsuleLight.cso"));
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
bool CCapsuleLightShader::CreateHS() {

	// 번역된 hlsl인 cso 로딩 후 PS 생성 : 추후 멀티스레드로 구현을 해야 할 부분
	auto loadPS = ReadCSOFile(TEXT("HSCapsuleLight.cso"));
	{
		m_pd3dDevice->CreateHullShader(
			&(loadPS[0])
			, loadPS.size()
			, nullptr
			, &m_hullShader
		);

	}

	if (m_hullShader) return true;

	return false;
}
bool CCapsuleLightShader::CreateDS() {

	// 번역된 hlsl인 cso 로딩 후 PS 생성 : 추후 멀티스레드로 구현을 해야 할 부분
	auto loadPS = ReadCSOFile(TEXT("DSCapsuleLight.cso"));
	{
		m_pd3dDevice->CreateDomainShader(
			&(loadPS[0])
			, loadPS.size()
			, nullptr
			, &m_domainShader
		);

	}

	if (m_domainShader) return true;

	return false;
}
bool CCapsuleLightShader::CreatePS() {

	// 번역된 hlsl인 cso 로딩 후 PS 생성 : 추후 멀티스레드로 구현을 해야 할 부분
	auto loadPS = ReadCSOFile(TEXT("PSCapsuleLight.cso"));
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


CCapsuleLightShader::CCapsuleLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderShader(pd3dDevice, pd3dDeviceContext) {

}
CCapsuleLightShader::~CCapsuleLightShader() {

}