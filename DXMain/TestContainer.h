#pragma once
#include "Container.h"
#include "RotateComponent.h"
#include "TestObject.h"
#include "TestMesh.h"
//#include "Shader.h"
#include "TestShader.h"
#include "InstancingShader.h"

class CTestContainer : public CContainer {
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();//컨테이너 초기화
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();
	//---------------------------dxobject---------------------------------

	virtual void RenderExcute();
private:

public:
	CTestContainer(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CTestContainer();
};
