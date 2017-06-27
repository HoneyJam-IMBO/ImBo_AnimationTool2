#pragma once
#include "Component.h"
#include "GameObject.h"

class CAnimateComponent :public CComponent {
public:
	//------------------------------내가 만들어 줘야 하는 함수---------------------------
	virtual bool Begin();
	virtual bool End();
	//virtual XMMATRIX Animate(XMMATRIX xmmtxWorld, float fTimeElapsed) { return XMMatrixIdentity(); }
	virtual void Animate(float fTimeElapsed) = 0;
	//------------------------------내가 만들어 줘야 하는 함수---------------------------

	//------------------------------컴포넌트 Update----------------------------
	virtual void Update(float fTimeElapsed);
	//------------------------------컴포넌트 Update----------------------------

	
protected:

public:

	CAnimateComponent();
	virtual ~CAnimateComponent();

};