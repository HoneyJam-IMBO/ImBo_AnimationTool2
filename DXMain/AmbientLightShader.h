#pragma once
#include "RenderShader.h"

class CAmbientLightShader : public CRenderShader {

public:
	//------------------------------shader-------------------------
	//begin func
	virtual bool CreateVS();
	virtual bool CreatePS();
	//begin func
	//------------------------------shader-------------------------
public:
	CAmbientLightShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CAmbientLightShader();
};