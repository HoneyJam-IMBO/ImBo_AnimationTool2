#pragma once
#include "RenderShader.h"

class CPostProcessingShader : public CRenderShader {

public:
	//------------------------------shader-------------------------
	//begin func
	virtual bool CreateVS();
	virtual bool CreatePS();
	//begin func
	//------------------------------shader-------------------------
public:
	CPostProcessingShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CPostProcessingShader();
};