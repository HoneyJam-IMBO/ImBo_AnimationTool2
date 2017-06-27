#pragma once
#include "RenderShader.h"

class CSpotLightShader : public CRenderShader {

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
	CSpotLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CSpotLightShader();
};