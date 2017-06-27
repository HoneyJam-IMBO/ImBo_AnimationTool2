#pragma once
#include "FileBasedMesh.h"
#include "StaticBuffer.h"
//#include "FbxJointData.h"
//test
#include "AnimationInfo.h"

class CAnimationMesh : public CFileBasedMesh {
public:
	//----------------------------dxobject-----------------------------
	bool Begin(UINT index = 0);
	virtual bool End();
	//----------------------------dxobject-----------------------------
private:

public:
	CAnimationMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CFileBasedMesh(pd3dDevice, pd3dDeviceContext) {};
	virtual ~CAnimationMesh() {};
};