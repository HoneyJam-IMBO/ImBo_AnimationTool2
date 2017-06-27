#include "stdafx.h"
#include "DownScaleFirstPass.h"

//begin func
bool CDownScaleFirstPassShader::CreateShaderValues() {

	//cs shader resource CB
	m_pFirstPassCB = CreateBuffer(sizeof(stFirstPassCB), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	
	return true;
}

bool CDownScaleFirstPassShader::CreateCS() {

	auto loadVS = ReadCSOFile(TEXT("CSDownScaleFirstPass.cso"));
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
bool CDownScaleFirstPassShader::ReleaseShaderValues() {
	if (m_pFirstPassCB) m_pFirstPassCB->Release();
	if (m_pFirstPassBuffer) m_pFirstPassBuffer->Release();
	if (m_pFirstPassSRV) m_pFirstPassSRV->Release();
	if (m_pFirstPassUAV) m_pFirstPassUAV->Release();

	//1
	if (m_pd3dtxtDownCasting) m_pd3dtxtDownCasting->Release();
	if (m_pd3dsrvDownCasting) m_pd3dsrvDownCasting->Release();
	if (m_pd3duavDownCasting) m_pd3duavDownCasting->Release();

	return true;
}

//set shadetstate func
void CDownScaleFirstPassShader::SetShaderValues() {
	//input은 이미 set되어 있음 srv..

	//uav
	//bloom
	//첫 패스에서 블룸 1/16scale texture를 set한다.
	ID3D11UnorderedAccessView* arrUAVs[2] = { m_pd3duavDownCasting, m_pFirstPassUAV };
	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 2, arrUAVs, (UINT*)(&arrUAVs));

	//constant
	ID3D11Buffer* arrConstBuffers[1] = { m_pFirstPassCB };
	m_pd3dDeviceContext->CSSetConstantBuffers(0, 1, arrConstBuffers);
	
	//cs도 set되어 있음
	//이후 dispatch
}
//clean func
void CDownScaleFirstPassShader::CleanShaderValues() {
	//uav set nullptr
	ID3D11UnorderedAccessView* pUAVs[2] = { nullptr, nullptr };
	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 2, pUAVs, (UINT*)(&pUAVs));

	//set next pass input
	//down scale 다 함 그럼 1/16 scale texture가 생성됨
	//이건 있다는 것이 확인됨

	ID3D11ShaderResourceView* pSRVs[2] = { m_pd3dsrvDownCasting, m_pFirstPassSRV };
	m_pd3dDeviceContext->CSSetShaderResources(0, 2, pSRVs);

}

void CDownScaleFirstPassShader::ResizeBuffer(UINT nWidth, UINT nHeight){
	m_nBufferWidth = nWidth;
	m_nBufferHeight = nHeight;
	
	//bloom
	if (m_pd3dtxtDownCasting) m_pd3dtxtDownCasting->Release();
	if (m_pd3dsrvDownCasting) m_pd3dsrvDownCasting->Release();
	if (m_pd3duavDownCasting) m_pd3duavDownCasting->Release();

	//----------------------------------------UAV Desc----------------------------------------------//
	D3D11_UNORDERED_ACCESS_VIEW_DESC d3dUAVDesc;
	ZeroMemory(&d3dUAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	d3dUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	d3dUAVDesc.Buffer.FirstElement = 0;
	d3dUAVDesc.Buffer.NumElements = static_cast<UINT>(nWidth*nHeight / 16);
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
	d3dTexture2DDesc.Width = static_cast<UINT>(nWidth / 4);
	d3dTexture2DDesc.Height = static_cast<UINT>(nHeight / 4);
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

	//1.
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDownCasting);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtDownCasting, &d3dSRVDesc, &m_pd3dsrvDownCasting);
	m_pd3dDevice->CreateUnorderedAccessView(m_pd3dtxtDownCasting, &d3dUAVDesc, &m_pd3duavDownCasting);

}

void CDownScaleFirstPassShader::SetFirstPassData(UINT width, UINT height, UINT nDownScaleGroups) {
	//첫번째 pass는 render target의 size가 변할때 마다 버퍼 size가 변해야 한다.
	//따라서 매번 변 할 때 마다. 새로 모든 것을 만들어 줘야 한다.
	m_nWidth = width;
	m_nHeight = height;
	m_nDownScaleGroup = nDownScaleGroups;


	//dispatch Thread Group set
	SetThreadGroup(m_nDownScaleGroup, 1, 1);
	//dispatch Thread Group set

	//shader resource
	//-------------------------------first pass----------------------
	//buffer
	if (m_pFirstPassBuffer) m_pFirstPassBuffer->Release();
	D3D11_BUFFER_DESC descBuffer;
	ZeroMemory(&descBuffer, sizeof(D3D11_BUFFER_DESC));
	descBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	descBuffer.StructureByteStride = sizeof(float);
	descBuffer.ByteWidth = m_nDownScaleGroup * descBuffer.StructureByteStride;
	descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_pd3dDevice->CreateBuffer(&descBuffer, nullptr, &m_pFirstPassBuffer);

	//uav
	if (m_pFirstPassUAV) m_pFirstPassUAV->Release();
	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	ZeroMemory(&descUAV, sizeof(descUAV));
	descUAV.Format = DXGI_FORMAT_UNKNOWN;
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descUAV.Buffer.NumElements = m_nDownScaleGroup;
	m_pd3dDevice->CreateUnorderedAccessView(m_pFirstPassBuffer, &descUAV, &m_pFirstPassUAV);

	//srv
	if (m_pFirstPassSRV) m_pFirstPassSRV->Release();
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = DXGI_FORMAT_UNKNOWN;
	descSRV.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	descSRV.Buffer.NumElements = m_nDownScaleGroup;
	m_pd3dDevice->CreateShaderResourceView(m_pFirstPassBuffer, &descSRV, &m_pFirstPassSRV);

	

}

void CDownScaleFirstPassShader::SetAdaptation(float fAdaptation){
	m_fAdaptation = fAdaptation;
}

void CDownScaleFirstPassShader::SetBloomThreshold(float fBloomThreshold){
	m_fBloomThreshold = fBloomThreshold;
}

//updater
void CDownScaleFirstPassShader::UpdateShaderState() {
	//적응값 있는 cb매 프레임 set
	//값 변경만 하면 매번 효과 적용됨 이게 더 직관적인것 같다.
	D3D11_MAPPED_SUBRESOURCE Data;
	m_pd3dDeviceContext->Map(m_pFirstPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
	stFirstPassCB* pFirstPassCB = (stFirstPassCB*)Data.pData;
	pFirstPassCB->nWidth = m_nWidth / 4;
	pFirstPassCB->nHeight = m_nHeight / 4;
	pFirstPassCB->nGroupSize = m_nDownScaleGroup;
	pFirstPassCB->fAdaptation = m_fAdaptation;
	pFirstPassCB->nTotalPixels = pFirstPassCB->nWidth * pFirstPassCB->nHeight;
	pFirstPassCB->fBloomThreshold = m_fBloomThreshold;
	m_pd3dDeviceContext->Unmap(m_pFirstPassCB, 0);
}

//생성자, 소멸자
CDownScaleFirstPassShader::CDownScaleFirstPassShader(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext) : CComputeShader(pd3dDevice, pd3dDeviceContext) {

}

CDownScaleFirstPassShader ::~CDownScaleFirstPassShader() {
}
