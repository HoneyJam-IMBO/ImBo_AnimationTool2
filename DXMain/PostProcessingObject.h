#pragma once
#include "Object.h"

class CPostProcessingObject : public CObject {
public:
	bool Begin();
	virtual bool End() { return CObject::End(); };

protected:
public:
	CPostProcessingObject() :CObject() { }
	virtual ~CPostProcessingObject() {}
};
