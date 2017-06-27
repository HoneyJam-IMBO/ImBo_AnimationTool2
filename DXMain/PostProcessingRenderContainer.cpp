#include "stdafx.h"
#include "PostProcessingRenderContainer.h"

//---------------------------dxobject---------------------------------
bool CPostProcessingRenderContainer::Begin() {

	//자기 shader set
	shared_ptr<CRenderShader> pShader = make_shared<CPostProcessingShader>(m_pd3dDevice, m_pd3dDeviceContext);
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	pShader->CreateVS(TEXT("VSPostProcessing.cso"), vertexDesc, 2);
	pShader->CreatePS(TEXT("PSPostProcessing.cso"));
	pShader->Begin();
	SetShader(pShader);

	//mesh
	shared_ptr<CMesh> pMesh = make_shared<CTestDeferredMesh>(m_pd3dDevice, m_pd3dDeviceContext);
	pMesh->Begin();
	SetMesh(pMesh);

	m_pCSDownScaleFirstPass = new CDownScaleFirstPassShader(m_pd3dDevice, m_pd3dDeviceContext);
	m_pCSDownScaleFirstPass->Begin();
	m_pCSDownScaleSecondPass = new CDownScaleSecondPassShader(m_pd3dDevice, m_pd3dDeviceContext);
	m_pCSDownScaleSecondPass->Begin();

	//bloom
	m_pCSBloom = new CCSBloom(m_pd3dDevice, m_pd3dDeviceContext);
	m_pCSBloom->Begin();

	//blur
	m_pCSHorizontalBlur = new CCSHorizontalBlur(m_pd3dDevice, m_pd3dDeviceContext);
	m_pCSHorizontalBlur->Begin();
	m_pCSVerticalBlur = new CCSVerticalBlur(m_pd3dDevice, m_pd3dDeviceContext);
	m_pCSVerticalBlur->Begin();

	m_pFinalPassCB = CreateBuffer(sizeof(stFinalPassCB), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	
	return true;
}
bool CPostProcessingRenderContainer::End() {

	if (m_pCSDownScaleFirstPass) {
		m_pCSDownScaleFirstPass->End();
		delete m_pCSDownScaleFirstPass;
	}
	if (m_pCSDownScaleSecondPass) {
		m_pCSDownScaleSecondPass->End();
		delete m_pCSDownScaleSecondPass;
	}

	//bloom
	if (m_pCSBloom) {
		m_pCSBloom->End();
		delete m_pCSBloom;
	}

	//blur
	if (m_pCSHorizontalBlur) {
		m_pCSHorizontalBlur->End();
		delete m_pCSHorizontalBlur;
	}
	if (m_pCSVerticalBlur) {
		m_pCSVerticalBlur->End();
		delete m_pCSVerticalBlur;
	}
	if (m_pFinalPassCB) m_pFinalPassCB->Release();

	return CRenderContainer::End();
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------



void CPostProcessingRenderContainer::UpdateShaderState() {

	m_pMesh->UpdateShaderState();
	m_pShader->UpdateShaderState();

}
void CPostProcessingRenderContainer::SetShaderState() {
	m_pMesh->SetShaderState();
	m_pShader->SetShaderState();
	m_pTexture->SetShaderState();//light tex /set cs, ps 
	
	//down scale <make AvgLum>
	m_pCSDownScaleFirstPass->ExcuteShaderState();//make 1/16 size tex
	m_pCSDownScaleSecondPass->ExcuteShaderState();

	//bloom
	m_pCSBloom->ExcuteShaderState();//make temp bloom tex
	
	//blur
	m_pCSHorizontalBlur->ExcuteShaderState();
	m_pCSVerticalBlur->ExcuteShaderState();
	//bloom

	//finalpass에 필요한 cb를 set
	m_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &m_pFinalPassCB);

}
void CPostProcessingRenderContainer::RenderExcute() {
	
	//그것을 이용하여
	//풀 스크린 드로우를 실행 한다.

	m_pMesh->RenderExcute(1);

}

void CPostProcessingRenderContainer::CleanShaderState() {

	m_pMesh->CleanShaderState();
	m_pShader->CleanShaderState();


	ID3D11Buffer* pBuffer[4] = { nullptr,nullptr,nullptr,nullptr };
	ID3D11ShaderResourceView* pSRVs[4] = { nullptr, nullptr,nullptr,nullptr };
	ID3D11UnorderedAccessView* pUAVs[4] = { nullptr,nullptr ,nullptr ,nullptr };

	m_pd3dDeviceContext->VSSetConstantBuffers(0, 4, pBuffer);
	m_pd3dDeviceContext->PSSetConstantBuffers(0, 4, pBuffer);
	m_pd3dDeviceContext->CSSetConstantBuffers(0, 4, pBuffer);
	
	m_pd3dDeviceContext->VSSetShaderResources(0, 4, pSRVs);
	m_pd3dDeviceContext->PSSetShaderResources(0, 4, pSRVs);
	m_pd3dDeviceContext->CSSetShaderResources(0, 4, pSRVs);

	m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 4, pUAVs, (UINT*)(&pUAVs));

	//적응을 하기위한 이전avgLum과 지금 계산한 avgLum을 교환
	m_pCSDownScaleSecondPass->SwapAdaptationBuff();

}

//적응값 추가
void CPostProcessingRenderContainer::SetFirstPassData(UINT width, UINT height) {
	
	m_nDownScaleGroup = (UINT)ceil((float)(width * height / 16) / 1024);
	m_nWidth = width;
	m_nHeight = height;

	//size 조정
	m_pCSDownScaleFirstPass->ResizeBuffer(m_nWidth, m_nHeight);
	m_pCSBloom->ResizeBuffer(m_nWidth, m_nHeight);
	m_pCSHorizontalBlur->ResizeBuffer(m_nWidth, m_nHeight);
	m_pCSVerticalBlur->ResizeBuffer(m_nWidth, m_nHeight);

	//group thread조정
	m_pCSBloom->SetThreadGroup(m_nDownScaleGroup, 1, 1);
	m_pCSHorizontalBlur->SetThreadGroup((UINT)ceil((m_nWidth / 4.0f) / (128.0f - 12.0f)), (UINT)ceil(m_nHeight / 4.0f), 1);
	m_pCSVerticalBlur->SetThreadGroup((UINT)ceil(m_nWidth / 4.0f), (UINT)ceil((m_nHeight / 4.0f) / (128.0f - 12.0f)), 1);

	m_pCSDownScaleFirstPass->SetFirstPassData(width, height, m_nDownScaleGroup);
}

void CPostProcessingRenderContainer::SetAdaptation(float fAdaptation){
	m_pCSDownScaleFirstPass->SetAdaptation(fAdaptation);
}
void CPostProcessingRenderContainer::SetBloomThreshold(float fBloomThreshold) {
	m_pCSDownScaleFirstPass->SetBloomThreshold(fBloomThreshold);
}

void CPostProcessingRenderContainer::SetFinalPassData(float fMiddleGrey, float fWhite, float fBloomScale) {
	m_fMeddleGrey = fMiddleGrey;
	m_fWhite = fWhite;
	m_fBloomScale = fBloomScale;

	D3D11_MAPPED_SUBRESOURCE Data;
	m_pd3dDeviceContext->Map(m_pFinalPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
	stFinalPassCB* pSecondPassCB = (stFinalPassCB*)Data.pData;
	pSecondPassCB->fMiddleGrey = m_fMeddleGrey;
	pSecondPassCB->fLumWhiteSqr = m_fWhite;
	pSecondPassCB->fLumWhiteSqr *= pSecondPassCB->fMiddleGrey;//Scale by the middle grey value
	pSecondPassCB->fLumWhiteSqr *= pSecondPassCB->fLumWhiteSqr;// Squre
	pSecondPassCB->fBloomScale = m_fBloomScale;

	m_pd3dDeviceContext->Unmap(m_pFinalPassCB, 0);

}

CPostProcessingRenderContainer::CPostProcessingRenderContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderContainer(pCamera, pd3dDevice, pd3dDeviceContext) {

}
CPostProcessingRenderContainer::~CPostProcessingRenderContainer() {

}