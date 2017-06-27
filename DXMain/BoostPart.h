#pragma once

#include "FunctionPart.h"

class CBoostPart : public CFunctionPart {
public:
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

	virtual ABILITY_VALUE GetAbilityValue() { return ABILITY_VALUE{ 0, m_fVelocity }; }
private:
	float m_fVelocity{ 0 };

public:
	CBoostPart();
	~CBoostPart();
};
