#pragma once

#include "Part.h"
#include "Object.h"
#include "BoostPart.h"

//--test--
#include "RotateComponent.h"

class CCore : public CPart {
public:
	//----------------------------object-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	//-----------------------------part inform func-------------------------------
	//core의 state변경 전달 - input, ai etc..
	//virtual void ChangeCoreState();
	//part의 state벼경 전달 - delete, new ..
	virtual void ChangePartState(int slot, part_state partState, CPart* pPart);
	void NewPart(int slot, CPart* pPart);
	//-----------------------------part inform func-------------------------------

	//-------------------------object Animate---------------------------
	//CObject Animate실행 - 이 함수 안에서 part들은 계층구조 Animate
	virtual void Animate(float fTimeElapsed);
	//-------------------------object Animate---------------------------

	//------------------------equip func--------------------------------
	virtual bool SetEquip(int slot, CPart* pPart);
	//------------------------equip func--------------------------------

	//-----------------------------prepare Render------------------------
	void GetPartsList(list<CPart*>& lParts);
	//-----------------------------prepare Render------------------------

	virtual void RegistToContainer();
private:
	//--------------------------------velocity--------------------------
	//boost 부품이 달려있는 위치에 따라 달라짐
	//rotarion velocity
	//rul -rul 0번부터 5번까지 
	float* m_pfRotationVelocity{ nullptr };
	//--------------------------------velocity--------------------------
	
	CPart** m_ppPart{ nullptr };//자신이 조립 가능한 part 6개

public:
	CCore();
	virtual ~CCore();
};
