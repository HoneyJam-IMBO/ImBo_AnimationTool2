#pragma once
#include "GameObject.h"
#include "RotateComponent.h"

class CPlane : public CGameObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

private:


public:
	CPlane();
	~CPlane();
};
