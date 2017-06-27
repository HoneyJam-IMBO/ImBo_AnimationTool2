#include "stdafx.h"
#include "Sampler.h"

bool CSampler::Begin(UINT Slot, UINT BindFlag, D3D11_TEXTURE_ADDRESS_MODE Mode, D3D11_FILTER Filter, D3D11_COMPARISON_FUNC ComparisionFunc, float MinLOD, float MaxLOD) {


	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	m_SamplerStartSlot = Slot;
	m_BindFlag = BindFlag;

	//sampler
	d3dSamplerDesc.AddressU = Mode;
	d3dSamplerDesc.AddressV = Mode;
	d3dSamplerDesc.AddressW = Mode;

	d3dSamplerDesc.Filter = Filter;
	d3dSamplerDesc.ComparisonFunc = ComparisionFunc;
	d3dSamplerDesc.MinLOD = MinLOD;
	d3dSamplerDesc.MaxLOD = MaxLOD;

	m_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pd3dSamplerState);
	
	return true;
}
bool CSampler::End() {

	if (m_pd3dSamplerState) {
		m_pd3dSamplerState->Release();
	}

	return true;
}

void CSampler::SetShaderState() {

		if (m_BindFlag & BIND_VS) {
			m_pd3dDeviceContext->VSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_DS) {
			m_pd3dDeviceContext->DSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_HS) {
			m_pd3dDeviceContext->HSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_GS) {
			m_pd3dDeviceContext->GSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_PS) {
			m_pd3dDeviceContext->PSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_CS) {
			m_pd3dDeviceContext->CSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
	
}
void CSampler::CleanShaderState() {
	ID3D11SamplerState* pSamplerState[1] = { nullptr };

		if (m_BindFlag & BIND_VS) {
			m_pd3dDeviceContext->VSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_DS) {
			m_pd3dDeviceContext->DSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_HS) {
			m_pd3dDeviceContext->HSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_GS) {
			m_pd3dDeviceContext->GSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_PS) {
			m_pd3dDeviceContext->PSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_CS) {
			m_pd3dDeviceContext->CSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}

}

void CSampler::UpdateShaderState() {

}


CSampler::CSampler(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("sampler", pd3dDevice, pd3dDeviceContext) { }
CSampler::~CSampler() { };

