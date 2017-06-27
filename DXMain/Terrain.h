#pragma once

#include "GameObject.h"


class CTerrain : public CGameObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

private:


public:
	CTerrain();
	~CTerrain();
};

