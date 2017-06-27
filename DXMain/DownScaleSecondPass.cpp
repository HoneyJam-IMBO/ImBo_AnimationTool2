#include "stdafx.h"
#include "DownScaleSecondPass.h"

//begin func
bool CDownScaleSecondPassShader::CreateShaderValues(){
	
	//tone mappint second pass shader resource
	//------------------------------second pass--------------------------
	//buffer
	D3D11_BUFFER_DESC descBuffer;
	ZeroMemory(&descBuffer, sizeof(D3D11_BUFFER_DESC));
	descBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	descBuffer.StructureByteStride = sizeof(float);
	descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descBuffer.ByteWidth = sizeof(float);
	m_pd3dDevice->CreateBuffer(&descBuffer, nullptr, &m_pSecondPassBuffer);
	m_pd3dDevice->CreateBuffer(&descBuffer, nullptr, &m_pPrevAvgLumBuffer);//이전 평균휘도 값
	//uav 
	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	ZeroMemory(&descUAV, sizeof(descUAV));
	descUAV.Format = DXGI_FORMAT_UNKNOWN;
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descUAV.Buffer.NumElements = 1;
	m_pd3dDevice->CreateUnorderedAccessView(m_pSecondPassBuffer, &descUAV, &m_pSecondPassUAV);
	m_pd3dDevice->CreateUnorderedAccessView(m_pPrevAvgLumBuffer, &descUAV, &m_pPrevAvgLumUAV);

	//srv
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = DXGI_FORMAT_UNKNOWN;
	descSRV.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	descSRV.Buffer.NumElements = 1;
	m_pd3dDevice->CreateShaderResourceView(m_pSecondPassBuffer, &descSRV, &m_pSecondPassSRV);
	m_pd3dDevice->CreateShaderResourceView(m_pPrevAvgLumBuffer, &descSRV, &m_pPrevAvgLumSRV);

	//변경되지 않을 것이니까 Begin에서 ThreadGroup set
	SetThreadGroup(1, 1, 1);
	return true;
}

bool CDownScaleSecondPassShader::CreateCS(){
	
	auto loadVS = ReadCSOFile(TEXT("CSDownScaleSecondPass.cso"));
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
bool CDownScaleSecondPassShader::ReleaseShaderValues(){
	if (m_pSecondPassBuffer) m_pSecondPassBuffer->Release();
	if (m_pSecondPassUAV) m_pSecondPassUAV->Release();
	if (m_pSecondPassSRV) m_pSecondPassSRV->Release();

	return true;
}

//set shadetstate func
void CDownScaleSecondPassShader::SetShaderValues(){
	//cs는 자동으로 clean up 됨 shader resource 들만 정리하면 됨

	//ID3D11ShaderResourceView* pSRVs[1] = { nullptr };
	//m_pd3dDeviceContext->PSSetShaderResources(1, 1, pSRVs);


	//input, constant buffer는 이미 set되어 있다.
	ID3D11UnorderedAccessView* arrUAVs[1] = { m_pSecondPassUAV };
	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, arrUAVs, (UINT*)(&arrUAVs));

	ID3D11ShaderResourceView* pSRVs[1] = { m_pPrevAvgLumSRV };
	m_pd3dDeviceContext->CSSetShaderResources(2, 1, pSRVs);
	//m_pd3dDeviceContext->PSSetShaderResources(5, 1, pSRVs);

}
//clean func
void CDownScaleSecondPassShader::CleanShaderValues(){
	
	ID3D11UnorderedAccessView* pUAVs[1] = { nullptr };
	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));

	//그리고 이제 ps에 hdr texture는 이미 set되어 있다.
	//0번에 1개가 set되어 있다.
	//1번에 지금껏 구한 평균 휘도 값을 set한다.
	m_pd3dDeviceContext->PSSetShaderResources(1, 1, &m_pSecondPassSRV);//tone mapping
	m_pd3dDeviceContext->CSSetShaderResources(1, 1, &m_pSecondPassSRV);//temp bloom tex 제작

}

void CDownScaleSecondPassShader::SwapAdaptationBuff(){
	//적응
	//계산 이 끝난 후에 두개의 버퍼의 값을 바꾸어 준다.
	ID3D11Buffer* pTempBuff = m_pPrevAvgLumBuffer;
	ID3D11ShaderResourceView* pTempSRV = m_pPrevAvgLumSRV;
	ID3D11UnorderedAccessView* pTempUAV = m_pPrevAvgLumUAV;

	m_pPrevAvgLumSRV = m_pSecondPassSRV;
	m_pPrevAvgLumUAV = m_pSecondPassUAV;
	m_pPrevAvgLumBuffer = m_pSecondPassBuffer;

	m_pSecondPassSRV = pTempSRV;
	m_pSecondPassUAV = pTempUAV;
	m_pSecondPassBuffer = pTempBuff;


	//계산 이 끝난 후에 두개의 버퍼의 값을 바꾸어 준다.

}


//updater
void CDownScaleSecondPassShader::UpdateShaderState() {

}

//생성자, 소멸자
CDownScaleSecondPassShader::CDownScaleSecondPassShader(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext) : CComputeShader(pd3dDevice, pd3dDeviceContext){

}

CDownScaleSecondPassShader::~CDownScaleSecondPassShader(){
}
