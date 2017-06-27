#pragma once
#include "RenderShader.h"

class CDirectionalLightShader : public CRenderShader {

public:
	//------------------------------shader-------------------------
	//begin func
	virtual bool CreateVS();
	virtual bool CreatePS();
	//begin func
	//------------------------------shader-------------------------
public:
	CDirectionalLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CDirectionalLightShader();
};