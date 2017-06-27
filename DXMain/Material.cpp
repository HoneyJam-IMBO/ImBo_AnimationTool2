#include "stdafx.h"
#include "Material.h"

bool CMaterial::Begin(XMFLOAT4 color, float specExp, float specIntensity) {
	m_infoMaterial.diffuseColor = color;
	m_infoMaterial.specData.x = specExp;
	m_infoMaterial.specData.y = specIntensity;

	m_pMaterialBuffer = CreateBuffer(sizeof(MATERIAL_INFO), 1, &m_infoMaterial, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	return true; 
}
bool CMaterial::End() { 
	if (m_pMaterialBuffer) m_pMaterialBuffer->Release();

	return true; 
}

void CMaterial::SetShaderState() {
	m_pd3dDeviceContext->PSSetConstantBuffers(PS_CB_MATERIAL, 1, &m_pMaterialBuffer);
}

void CMaterial::CleanShaderState() {

}

void CMaterial::UpdateShaderState() {
	//m_pd3dDeviceContext->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &m_infoMaterial, 0, 0);
}


CMaterial::CMaterial(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("material", pd3dDevice, pd3dDeviceContext){ }
CMaterial::~CMaterial() { };

