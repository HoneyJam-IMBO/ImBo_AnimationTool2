#pragma once
#include "AnimateComponent.h"

class CRotateComponent : public CAnimateComponent {
public:
	//------------------------------내가 만들어 줘야 하는 함수---------------------------
	virtual bool Begin();
	virtual bool End();
	//virtual XMMATRIX Animate(XMMATRIX xmmtxWorld, float fTimeElapsed);
	virtual void Animate(float fTimeElapsed);
	//------------------------------내가 만들어 줘야 하는 함수---------------------------
protected:
	float m_fRotate{ 0 };
	
public:

	CRotateComponent();
	virtual ~CRotateComponent();
};