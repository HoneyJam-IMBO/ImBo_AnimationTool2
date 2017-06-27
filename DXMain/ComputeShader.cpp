#include "stdafx.h"
#include "ComputeShader.h"


bool CComputeShader::Begin() {
	if (false == CreateCS()) return false;
	
	if (false == CreateShaderValues()) return false;

	return true;
}
bool CComputeShader::End() {
	if (m_computeShader)		m_computeShader->Release();
	
	if(false == ReleaseShaderValues()) return false;

	return true;
}

void CComputeShader::SetShaderState() {
	// cs
	if (m_computeShader) {
		m_pd3dDeviceContext->CSSetShader(m_computeShader, nullptr, 0);
	}

	//virtual
	SetShaderValues();
}
void CComputeShader::CleanShaderState() {
	// cs
	if (m_computeShader) {
		m_pd3dDeviceContext->CSSetShader(nullptr, nullptr, 0);
	}
	
	//virtual
	CleanShaderValues();
}

void CComputeShader::UpdateShaderState() {

}

void CComputeShader::ExcuteShaderState(){
	UpdateShaderState();
	SetShaderState();
	m_pd3dDeviceContext->Dispatch(m_ThreadGroupX, m_ThreadGroupY, m_ThreadGroupZ);
	CleanShaderState();
}

void CComputeShader::ResizeBuffer(UINT nWidth, UINT nHeight) {
	m_nBufferWidth = nWidth;
	m_nBufferHeight = nHeight;
}

void CComputeShader::SetShaderValues()
{
}

void CComputeShader::CleanShaderValues()
{
}

void CComputeShader::SetThreadGroup(UINT x, UINT y, UINT z)
{
	m_ThreadGroupX = x;
	m_ThreadGroupY = y;
	m_ThreadGroupZ = z;
}


CComputeShader::CComputeShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CShader(pd3dDevice, pd3dDeviceContext) {

}
CComputeShader::~CComputeShader() {

}