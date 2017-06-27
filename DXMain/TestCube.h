#pragma once
#include "GameObject.h"
#include "RotateComponent.h"

class CTestCube : public CGameObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

	virtual void SetPosition(XMVECTOR pos);
private:


public:
	CTestCube();
	~CTestCube();
};
