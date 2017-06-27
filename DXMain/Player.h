#pragma once
#include "GameObject.h"

class CPlayer : public CGameObject{
public:
	//----------------------------object-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------
	
	//회전
	//virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	//이동
	//virtual void Move(XMVECTOR xmvDir, float fDistance);

	//----------------------------------------input------------------------
	virtual void ProcessInput(float fTimeElapsed, UCHAR* pKeyBuffer, float cxDelta, float cyDelta);
	//----------------------------------------input------------------------

	CGameObject* GetControlObject() { return m_pControlObject; }
private:
	//자신이 조종하는 객체
	CGameObject* m_pControlObject{ nullptr };

public:
	CPlayer();
	virtual ~CPlayer();

};