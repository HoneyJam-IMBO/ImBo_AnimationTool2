#pragma once
#include "DXObject.h"
#include "Texture.h"

//재질은 texture와 재질 정보를 갖는 cbuffer를 가진다.
//texture - srv
//sampler
//material struct
struct MATERIAL_INFO {

	XMFLOAT4 diffuseColor;
	XMFLOAT4 specData;

};

class CMaterial : public DXObject {
public:
	bool Begin(XMFLOAT4 color, float specExp, float specIntensity);
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();
protected:
	ID3D11Buffer* m_pMaterialBuffer{ nullptr };

	MATERIAL_INFO m_infoMaterial{};

public:
	CMaterial(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CMaterial();

};
