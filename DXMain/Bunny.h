#pragma once
#include "GameObject.h"
#include "RotateComponent.h"

struct CTestStruct {
	CFileBasedMesh* pMesh;
	CFbxJointData* pJoint;
};
class CBunny : public CGameObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------
	virtual void Animate(float fTimeElapsed);

	virtual void SetPosition(XMVECTOR pos);
	//virtual void PickingProc();
	//void SelectAnimationProc();
private:
	bool test;

public:
	CBunny();
	~CBunny();
};
