#pragma once
#include "DXObject.h"


class CSampler : public DXObject {
public:
	bool Begin(UINT Slot = 0, UINT BindFlags = BIND_PS, D3D11_TEXTURE_ADDRESS_MODE Mode = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_COMPARISON_FUNC ComparisionFunc = D3D11_COMPARISON_NEVER, float MinLOD = 0, float MaxLOD = 0);
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();


protected:
	//smapler 1°³
	ID3D11SamplerState*				m_pd3dSamplerState{ nullptr };
	UINT							m_SamplerStartSlot{ 0 };
	UINT							m_BindFlag{ 0 };

public:
	CSampler(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CSampler();

};