#pragma once
#include "GameObject.h"
#include "RotateComponent.h"

class CTestObject : public CGameObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------
	virtual void Animate(float fTimeElapsed);

	virtual void SetPosition(XMVECTOR pos);
	//virtual void PickingProc();
private:


public:
	CTestObject();
	~CTestObject();
};
