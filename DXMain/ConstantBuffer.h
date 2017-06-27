#pragma once
#include "Buffer.h"

class CConstantBuffer : public CBuffer {
public:
	bool Begin(UINT Object, UINT BufferStride, UINT Slot = 0, UINT BindFlag = BIND_VS, UINT Offset = 0);
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();
	
protected:
	ID3D11Buffer* CreateConstantBuffer(UINT nByteWidth);

	//buffer info
	UINT							m_Slot{ 0 };

	//bind flag default is none 반드시 set해줘야 한다.
	UINT							m_BindFlag{ 0 };
protected:
	
public:
	CConstantBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CConstantBuffer();

};