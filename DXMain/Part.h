#pragma once

#include "GameObject.h"

//enum part_id {
//	PART_CORE,
//	PART_ADAPTOR,
//	PART_WEAPON,
//	PART_BOOST,
//	PART_END
//};
enum part_state {
	STATE_NEW,
	STATE_DELETE,
	STATE_END
};

struct ABILITY_VALUE {
	float m_fsize{ 0 };
	float m_fVelocity{ 0 };
	ABILITY_VALUE(float size, float velocity) : m_fsize(size), m_fVelocity(velocity) {}
};

class CPart : public CGameObject {
public:
	//----------------------------object-----------------------------
	virtual bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	//--------------------------part inform func----------------------------
	//core의 state변경 전달 - input, ai etc..
	virtual void ChangeCoreState() { };
	//part의 state변경 전달 - delete, new ..etc 
	virtual void ChangePartState(int slot, part_state partState, CPart* pPart) { };
	//--------------------------part inform func----------------------------

	//--------------------------part Animate------------------------------
	//part들만의 특별한 계층구조 Animate 
	//중심Animate는 Core다.
	virtual void Animate(float fTimeElapsed, XMMATRIX& xmmtxParent) { };
	//--------------------------part Animate------------------------------

	//--------------------------prepare Render-----------------------
	//자신이 가진 부품 전부 list에 set core에서 시작해서 모든 부품을 돌면서 list에 집어 넣는다.
	virtual void GetPartsList(list<CPart*>& lParts) {};
	//--------------------------prepare Render-----------------------


	//---------------------------equip-------------------------------
	//자신의 슬롯에 part장착 및 상대 Equiped함수 호출
	bool Equip(int slot, CPart* pAdaptor);
	//상대 part를 parent part로써 설정하고 parent위치에 따라 offset을 주어 자신의 위치를 조정
	bool Equiped(int slot, CPart* pAdaptor);
	//---------------------------equip-------------------------------


	//-------------------------set getter--------------------------------
	//part render container를 위한 id
	//part_id GetPartID() { return m_partID; }
	//offset지정을 위한 size설정 
	float GetSize() { return m_fSize; }
	virtual ABILITY_VALUE GetAbilityValue() { return ABILITY_VALUE(0,0); };

	//localmtx
	void SetLocalMtx(XMMATRIX mtxLocal) { XMStoreFloat4x4(&m_xmf4x4Local, mtxLocal); }
	XMMATRIX GetLocalMtx() { return XMLoadFloat4x4(&m_xmf4x4Local); }
	//------------------------equip func--------------------------------
	//해당 슬롯에 part장착!
	virtual bool SetEquip(int slot, CPart* pParent) { return false; };
	//------------------------equip func--------------------------------

	//------------------------equiped func--------------------------------
	//parent
	//parent를 기준으로 offset을 두어 자신의 위치 조정
	virtual bool SetLocalLocation(int slot, CPart* pParent) { return false; };
	//parent를 set하고 자신의 위치를 parent의 size를 offset으로 두어 위치조정
	virtual bool SetParent(int slot, CPart* pParent) { return false; };
	//------------------------equiped func--------------------------------

	virtual CPart* GetParent() { return m_pParent; }
	//-------------------------set getter--------------------------------

	//virtual void RegistToContainer(mapRenderContainer& mRenderContainer);

protected:
	//자신의 hp
	float m_fHP{ 0 };
	float m_fSize{ 0 };

	//part_id
	//part_id m_partID{ PART_END };

	//자신의 부모 포인터
	CPart* m_pParent{ nullptr };

	//-------------------------------localmtx----------------------------
	XMFLOAT4X4 m_xmf4x4Local;
	//-------------------------------localmtx----------------------------
public:
	CPart() : CGameObject("part", tag::TAG_DYNAMIC_OBJECT) { };
	virtual ~CPart() { };
};