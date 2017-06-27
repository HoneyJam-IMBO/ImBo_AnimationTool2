#include "stdafx.h"
#include "InstanceBuffer.h"

bool CInstanceBuffer::Begin(UINT Object, UINT BufferStride, UINT Offset) {
	
		m_nObject = Object;
		m_BufferStride = BufferStride;
		m_ByteWidth = m_nObject * m_BufferStride;
		m_Offset = Offset;
		//create buffer
		m_pd3dBuffer = CreateInstanceBuffer(m_ByteWidth);
	
	return true;
}

bool CInstanceBuffer::End() {

	return true;
}

//instance buffer는 set/ clean 그런거 안해도 됨
void CInstanceBuffer::SetShaderState() {

}
void CInstanceBuffer::CleanShaderState() {

}

void CInstanceBuffer::UpdateShaderState() {

}


//초기 정보 없는 instanceBuffer제작
ID3D11Buffer* CInstanceBuffer::CreateInstanceBuffer(UINT nByteWidth) {

	ID3D11Buffer *pd3dInstanceBuffer = NULL;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	/*버퍼의 초기화 데이터가 없으면 동적 버퍼로 생성한다. 즉, 나중에 매핑을 하여 내용을 채우거나 변경한다.*/
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nByteWidth;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &pd3dInstanceBuffer);
	return(pd3dInstanceBuffer);

}


CInstanceBuffer::CInstanceBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CBuffer(pd3dDevice, pd3dDeviceContext) { }
CInstanceBuffer::~CInstanceBuffer() { };

