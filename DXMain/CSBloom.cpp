#include "stdafx.h"
#include "CsBloom.h"

//begin func
bool CCSBloom::CreateShaderValues() {
	
	return true;
}

bool CCSBloom::CreateCS() {

	auto loadVS = ReadCSOFile(TEXT("CSBloom.cso"));
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
bool CCSBloom::ReleaseShaderValues() {
	if (m_pd3dtxtTempBloom) m_pd3dtxtTempBloom->Release();
	if (m_pd3dsrvTempBloom)m_pd3dsrvTempBloom->Release();
	if (m_pd3duavTempBloom)m_pd3duavTempBloom->Release();
	return true;
}

//set shadetstate func
void CCSBloom::SetShaderValues() {
	ID3D11UnorderedAccessView* pUAVs[1] = { m_pd3duavTempBloom };
	//1번에는 0번 tmp texture를
	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));

}
//clean func
void CCSBloom::CleanShaderValues() {

	ID3D11UnorderedAccessView* pUAVs[1] = { nullptr };
	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));
	//bloom을 위한 텍스쳐 제작 끝.
	/*
	이제 추출한 밝은 부분에 블러를 입여준다.
	*/
	//blur 1
	//set
	ID3D11ShaderResourceView* pSRVs[1] = { m_pd3dsrvTempBloom };
	m_pd3dDeviceContext->CSSetShaderResources(0, 1, pSRVs);
}

//updater
void CCSBloom::UpdateShaderState() {
	
}

void CCSBloom::ResizeBuffer(UINT nWidth, UINT nHeight){
	m_nBufferWidth = nWidth;
	m_nBufferHeight = nHeight;

	if (m_pd3dtxtTempBloom) m_pd3dtxtTempBloom->Release();
	if (m_pd3dsrvTempBloom)m_pd3dsrvTempBloom->Release();
	if (m_pd3duavTempBloom)m_pd3duavTempBloom->Release();


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

	//2.
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtTempBloom);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtTempBloom, &d3dSRVDesc, &m_pd3dsrvTempBloom);
	m_pd3dDevice->CreateUnorderedAccessView(m_pd3dtxtTempBloom, &d3dUAVDesc, &m_pd3duavTempBloom);
}

//생성자, 소멸자
CCSBloom::CCSBloom(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext) : CComputeShader(pd3dDevice, pd3dDeviceContext) {

}

CCSBloom::~CCSBloom() {
}
