#pragma once
class CGameObject;

enum component_id {
	ROTATE,
	COMPONENTID_END
};

enum family_id {
	ANIMATE,
	FAMILYID_END
};


class CComponent{
public:
	virtual bool Begin();
	virtual bool End();

	//해당 컴포넌트 Update
	virtual void Update(float fTimeElapsed) = 0;

	//familyid, 내 id get
	const family_id GetFamilyID() { return m_FamilyID; };
	const component_id GetID() { return m_ID; };

	//owner set, get
	void SetOwner(CGameObject* pObject);
	CGameObject* GetOwner() { return m_pOwner; }
protected:
	//컴포넌트 id
	component_id m_ID;

	//같은 종류의 컴포넌트를 모아놓은 패밀리 id
	//ex) geometry 사용한 render와 그냥 render
	family_id m_FamilyID;

	CGameObject* m_pOwner;

public:

	CComponent();
	virtual ~CComponent();
};

