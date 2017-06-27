#pragma once
#include "ConstantBuffer.h"

class CStaticBuffer : public CConstantBuffer {
public:
	bool Begin(UINT Object, UINT BufferStride, void* pData, UINT Slot = 0, UINT BindFlag = BIND_VS, UINT Offset = 0);
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

protected:
	ID3D11Buffer* CreateStaticBuffer(UINT nByteWidth, void* pData);

public:
	CStaticBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CStaticBuffer();

};