#pragma once
#include "RenderShader.h"

class CPointLightShader : public CRenderShader {

public:
	//------------------------------shader-------------------------
	//begin func
	virtual bool CreateVS();
	virtual bool CreateHS();
	virtual bool CreateDS();
	virtual bool CreatePS();
	//begin func
	//------------------------------shader-------------------------
public:
	CPointLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CPointLightShader();
};