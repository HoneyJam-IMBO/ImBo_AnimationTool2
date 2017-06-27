#pragma once
#include "DXObject.h"
#include "Ambient.h"

#define MAX_AMBIENT_LIGHT_NUM 1
#define MAX_DIRECTIONAL_LIGHT_NUM 1

class CSceneLight : public DXObject {
public:

	//--------------------dxobject-------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void SetnullptrShaderState();

	virtual void UpdateShaderState();
	//--------------------dxobject-------------------

	void SetAmbientLight(XMFLOAT4 m_AmbientDown, XMFLOAT4 m_AmbientRange, XMFLOAT4 m_AmbientUp, XMFLOAT4 m_AmbientColor);
private:
	//ambient test
	CAmbient** m_ppAmbient{ nullptr };
	int m_nAmbient{ 0 };
	//ambient test

	ID3D11Buffer* m_pSceneLightBuffer{ nullptr };
public:
	CSceneLight(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CSceneLight();
};