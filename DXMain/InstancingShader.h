#pragma once
#include "RenderShader.h"

class CInstancingShader : public CRenderShader {

public:
	//------------------------------shader-------------------------
	//begin func
	virtual bool CreateVS();
	virtual bool CreatePS();
	//begin func
	//------------------------------shader-------------------------
public:
	CInstancingShader(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CInstancingShader();
};