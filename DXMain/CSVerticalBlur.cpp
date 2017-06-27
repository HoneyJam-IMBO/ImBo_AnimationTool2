#include "stdafx.h"
#include "CSVerticalBlur.h"

//begin func
bool CCSVerticalBlur::CreateShaderValues() {

	return true;
}

bool CCSVerticalBlur::CreateCS() {

	auto loadVS = ReadCSOFile(TEXT("CSVerticalBlur.cso"));
	{
		m_pd3dDevice->CreateComputeShader(
			&(loadVS[0])
			, loadVS.size()
			, nullptr
			, &m_computeShader
		);

	}

	if (m_computeShader) return true;

	return false;

}

//end func
bool CCSVerticalBlur::ReleaseShaderValues() {

	if (m_pd3dtxtBloom) m_pd3dtxtBloom->Release();
	if (m_pd3dsrvBloom)m_pd3dsrvBloom->Release();
	if (m_pd3duavBloom)m_pd3duavBloom->Release();

	return true;
}

//set shadetstate func
void CCSVerticalBlur::SetShaderValues() {
	ID3D11UnorderedAccessView* pUAVs[1] = { m_pd3duavBloom };
	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));

}
//clean func
void CCSVerticalBlur::CleanShaderValues() {

	//짜잔 이제 bloom blur이미지가 완성되었다.
	//test
	ID3D11UnorderedAccessView* pUAVs[1] = { nullptr };
	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));

	m_pd3dDeviceContext->PSSetShaderResources(2, 1, &m_pd3dsrvBloom);

}

//updater
void CCSVerticalBlur::UpdateShaderState() {

}

void CCSVerticalBlur::ResizeBuffer(UINT nWidth, UINT nHeight){

	m_nBufferWidth = nWidth;
	m_nBufferHeight = nHeight;

	if (m_pd3dtxtBloom) m_pd3dtxtBloom->Release();
	if (m_pd3dsrvBloom)m_pd3dsrvBloom->Release();
	if (m_pd3duavBloom)m_pd3duavBloom->Release();


	//----------------------------------------UAV Desc----------------------------------------------//
	D3D11_UNORDERED_ACCESS_VIEW_DESC d3dUAVDesc;
	ZeroMemory(&d3dUAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	d3dUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	d3dUAVDesc.Buffer.FirstElement = 0;
	d3dUAVDesc.Buffer.NumElements = nWidth*nHeight / 16;
	//----------------------------------------UAV Desc----------------------------------------------//
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
	d3dTexture2DDesc.Width = nWidth / 4;
	d3dTexture2DDesc.Height = nHeight / 4;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	//----------------------------------------TextUre Desc-----------------------------------------//
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dUAVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	//3.
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtBloom);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtBloom, &d3dSRVDesc, &m_pd3dsrvBloom);
	m_pd3dDevice->CreateUnorderedAccessView(m_pd3dtxtBloom, &d3dUAVDesc, &m_pd3duavBloom);
	//bloom texture 제작

}

//생성자, 소멸자
CCSVerticalBlur::CCSVerticalBlur(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext) : CComputeShader(pd3dDevice, pd3dDeviceContext) {

}

CCSVerticalBlur::~CCSVerticalBlur() {
}
