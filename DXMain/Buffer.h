#pragma once
#include "DXObject.h"


class CBuffer : public DXObject {
public:
	bool Begin() { return true; };
	virtual bool End() { return true; };

	virtual void SetShaderState() {};
	virtual void CleanShaderState() {};

	virtual void UpdateShaderState() {};

	//map
	void* Map();
	void Unmap();
	//unmap

	//getter
	UINT GetBufferStride() { return m_BufferStride; }
	UINT GetnObject() { return m_nObject; }
	UINT GetOffset() { return m_Offset; }
	ID3D11Buffer* GetBuffer() { return m_pd3dBuffer; }

private:

protected:

	//buffer 1개
	ID3D11Buffer*					m_pd3dBuffer{ nullptr };

	//map data 1개
	D3D11_MAPPED_SUBRESOURCE		m_MappedData{ nullptr };
	void*							m_pData{ nullptr };

	//buffe info 1개
	UINT							m_nObject{ 0 };
	UINT							m_BufferStride{ 0 };
	UINT							m_ByteWidth{ 0 };
	UINT							m_Offset{ 0 };
public:
	CBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CBuffer();

};

/*
//버퍼가 있었으면
if (m_ppBuffers) {
//이전 버퍼들 release

//새로운 버퍼 배열 제작
new_ppBuffers = new shared_ptr<CBuffer>[m_nBuffers];
//이전 값 복사
int i = 0;
for (i; i < m_nBuffers; ++i) {
new_ppBuffers[i] = m_ppBuffers[i];
}
//현재 들어온 값 복사
int j;
for (int j = 0; j < nBuffers; ++j) {
new_ppBuffers[m_nBuffers + j] = ppBuffers[j];
}
//현재 버퍼 지우고
delete[] m_ppBuffers;

//값 갱신
m_ppBuffers = new_ppBuffers;
m_nBuffers = new_nBuffers;
}
else {
//값 갱신
m_ppBuffers = ppBuffers;
m_nBuffers = nBuffers;
}
*/