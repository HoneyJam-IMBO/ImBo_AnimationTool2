#pragma once

#include "Part.h"
#include "Object.h"
#include "BoostPart.h"

//--test--
#include "RotateComponent.h"

class CAdaptorPart : public CPart {
public:
	//----------------------------object-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	//-----------------------------part inform func-------------------------------
	//virtual void ChangeCoreState();
	virtual void ChangePartState(int slot, part_state partState, CPart* pPart);
	//-----------------------------part inform func-------------------------------

	//--------------------------part Animate------------------------------
	virtual void Animate(float fTimeElapsed, XMMATRIX& xmmtxParents);
	//--------------------------part Animate------------------------------

	//-----------------------------prepare Render------------------------
	void GetPartsList(list<CPart*>& lParts);
	//-----------------------------prepare Render------------------------

	//------------------------equip func--------------------------------
	virtual bool SetEquip(int slot, CPart* pPart);
	//------------------------equip func--------------------------------
	
	//--------------------------equiped func--------------------------
	//parent
	virtual bool SetParent(int slot, CPart* pParent);
	//parent를 기준으로 offset을 두어 자신의 위치 조정
	virtual bool SetLocalLocation(int slot, CPart* pParent);
	//--------------------------equiped func--------------------------

	virtual void RegistToContainer();

private:
	CPart** m_ppPart{ nullptr };//자신이 가지는 부품 6개
	int m_nParentSlot{ 0 };//자신의 부모 부품 이 있는 슬롯

public:
	CAdaptorPart();
	virtual ~CAdaptorPart();
};

/*
core -> part관리 /part = 5개의 자식을 가지는 트리 구조
brunch = adaptorpart 자신의 변경점을 core에게 알리는 파츠 (core를 변수로 가짐)
leaf = functionpart 기능을 가진 파츠
*/