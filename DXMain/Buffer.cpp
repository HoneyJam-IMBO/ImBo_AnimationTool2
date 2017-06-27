#include "stdafx.h"
#include "Buffer.h"


void* CBuffer::Map() {
		m_pd3dDeviceContext->Map(m_pd3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_MappedData);
		m_pData = m_MappedData.pData;

	return m_pData;
}

void CBuffer::Unmap() {
	
		m_pd3dDeviceContext->Unmap(m_pd3dBuffer, 0);
}


CBuffer::CBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("buffer", pd3dDevice, pd3dDeviceContext) { }
CBuffer::~CBuffer() { };

