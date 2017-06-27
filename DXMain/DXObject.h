#pragma once
#include "Object.h"

class DXObject : public CObject {
public:
	bool Begin() { return true; }
	virtual bool End() { return true; }

	virtual void SetShaderState(){ }
	virtual void CleanShaderState(){ }
	 
	virtual void UpdateShaderState(){ }

	ID3D11Buffer* CreateBuffer(UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags);

protected:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;

public:
	DXObject(string name, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) 
		: m_pd3dDevice(pd3dDevice), m_pd3dDeviceContext(pd3dDeviceContext), CObject(name) { }
	virtual ~DXObject() { };
};
