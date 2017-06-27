#pragma once

#include "Part.h"


class CFunctionPart : public CPart {
public:
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

	//---------------------------parts animate----------------------
	virtual void Animate(float fTimeElapsed, XMMATRIX& xmmtxParents);
	//---------------------------parts animate----------------------

	//--------------------------prepare Render----------------------
	virtual void GetPartsList(list<CPart*>& lParts);
	//--------------------------prepare Render----------------------

	//--------------------------equiped func--------------------------
	virtual bool SetParent(int slot, CPart* pParent);
	virtual bool SetLocalLocation(int slot, CPart* pParent);
	//--------------------------equiped func--------------------------
public:
	CFunctionPart() : CPart() {}
	virtual ~CFunctionPart() {}
};