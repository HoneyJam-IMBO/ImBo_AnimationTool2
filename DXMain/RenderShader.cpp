#include "stdafx.h"
#include "RenderShader.h"


bool CRenderShader::Begin() {
	
	if (false == CreateShaderValues()) return false;

	return true;
}
bool CRenderShader::End() {
	if (m_vertexShader)			m_vertexShader->Release();
	if (m_geometryShader)		m_geometryShader->Release();
	if (m_hullShader)			m_hullShader->Release();
	if (m_domainShader)			m_domainShader->Release();
	if (m_pixelShader)			m_pixelShader->Release();

	if (m_inputLayout)			m_inputLayout->Release();

	return true;
}

void CRenderShader::SetShaderState() {
	// Input Layout
	m_pd3dDeviceContext->IASetInputLayout(m_inputLayout);

	// vs
	if (m_vertexShader) {
		m_pd3dDeviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	}

	// gs
	if (m_geometryShader) {
		m_pd3dDeviceContext->GSSetShader(m_geometryShader, nullptr, 0);
	}

	// hs
	if (m_hullShader) {
		m_pd3dDeviceContext->HSSetShader(m_hullShader, nullptr, 0);
	}

	//ds 
	if (m_domainShader) {
		m_pd3dDeviceContext->DSSetShader(m_domainShader, nullptr, 0);
	}

	// ps
	if (m_pixelShader) {
		m_pd3dDeviceContext->PSSetShader(m_pixelShader, nullptr, 0);
	}
}
void CRenderShader::CleanShaderState() {
	// ps
	if (m_vertexShader) {
		m_pd3dDeviceContext->VSSetShader(nullptr, nullptr, 0);
	}

	// gs
	if (m_geometryShader) {
		m_pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	}

	// hs
	if (m_hullShader) {
		m_pd3dDeviceContext->HSSetShader(nullptr, nullptr, 0);
	}

	//ds
	if (m_domainShader) {
		m_pd3dDeviceContext->DSSetShader(nullptr, nullptr, 0);
	}

	// ps
	if (m_pixelShader) {
		m_pd3dDeviceContext->PSSetShader(nullptr, nullptr, 0);
	}
}

void CRenderShader::UpdateShaderState() {

}

bool CRenderShader::CreateVS(LPCTSTR name, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement){
	auto loadShader = ReadCSOFile(name);
	{
		m_pd3dDevice->CreateVertexShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_vertexShader
		);

		if (vsDesc) {
			m_pd3dDevice->CreateInputLayout(
				vsDesc
				, nElement
				, &(loadShader[0])
				, loadShader.size()
				, &m_inputLayout
			);
		}
	}

	if (m_vertexShader) return true;

	return false;
}

bool CRenderShader::CreateGS(LPCTSTR name){

	auto loadShader = ReadCSOFile(name);
	{
		m_pd3dDevice->CreateGeometryShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_geometryShader
		);

	}

	if (m_geometryShader) return true;

	return false;
}

bool CRenderShader::CreateHS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		m_pd3dDevice->CreateHullShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_hullShader
		);

	}

	if (m_hullShader) return true;

	return false;
}

bool CRenderShader::CreateDS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		m_pd3dDevice->CreateDomainShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_domainShader
		);

	}

	if (m_domainShader) return true;

	return false;
}

bool CRenderShader::CreatePS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		m_pd3dDevice->CreatePixelShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_pixelShader
		);

	}

	if (m_pixelShader) return true;

	return false;
}

void CRenderShader::SetShaderData(LPCTSTR vsName, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement, LPCTSTR gsName, LPCTSTR hsName, LPCTSTR dsName, LPCTSTR psName){
	if(vsName) CreateVS(vsName, vsDesc, nElement);
	if(gsName) CreateGS(gsName);
	if(dsName) CreateDS(dsName);
	if(hsName) CreateHS(hsName);
	if(psName) CreatePS(psName);
}


CRenderShader::CRenderShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CShader(pd3dDevice, pd3dDeviceContext) {

}
CRenderShader::~CRenderShader() {

}