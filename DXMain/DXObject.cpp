#include "stdafx.h"
#include "DXObject.h"

ID3D11Buffer* DXObject::CreateBuffer(UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags) {
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = d3dUsage;
	d3dBufferDesc.ByteWidth = nStride * nElements;
	d3dBufferDesc.BindFlags = nBindFlags;
	d3dBufferDesc.CPUAccessFlags = nCPUAccessFlags;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;

	ID3D11Buffer *pd3dBuffer = nullptr;
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, pBufferData? &d3dBufferData : nullptr, &pd3dBuffer);
	return(pd3dBuffer);
}