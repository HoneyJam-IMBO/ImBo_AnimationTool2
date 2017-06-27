#pragma once

#include "FunctionPart.h"

class CWeaponPart : public CFunctionPart {
public:
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

public:
	CWeaponPart();
	~CWeaponPart();
};
