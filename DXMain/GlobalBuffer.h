#pragma once
#include "ConstantBuffer.h"
#include "GlobalObject.h"
class CGlobalBuffer : public CConstantBuffer {
public:
	//object
	bool Begin(UINT BufferStride, UINT Slot = 0, UINT BindFlag = BIND_VS, UINT Offset = 0);
	virtual bool End();
	//object

private:

public:
	CGlobalBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CGlobalBuffer();

};