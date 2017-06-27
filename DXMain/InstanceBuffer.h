#pragma once
#include "Buffer.h"


class CInstanceBuffer : public CBuffer {
public:
	bool Begin(UINT Object, UINT BufferStride, UINT Offset = 0);
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

private:
	ID3D11Buffer* CreateInstanceBuffer(UINT nByteWidth);

protected:

public:
	CInstanceBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CInstanceBuffer();

};