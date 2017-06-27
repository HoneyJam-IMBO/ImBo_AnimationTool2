#include "stdafx.h"
#include "AnimateComponent.h"


bool CAnimateComponent::Begin() {

	return TRUE;
};

bool CAnimateComponent::End() {

	//자신의 소유자
	m_pOwner = NULL;

	return CComponent::End();
};

void CAnimateComponent::Update(float fTimeElapsed) {

	//1. owner mtx get
	//2. animate
	//3. owner mtx set
	Animate(fTimeElapsed);
	//m_pOwner->SetWorldMtx(Animate(m_pOwner->GetWorldMtx(), fTimeElapsed));
}

CAnimateComponent::CAnimateComponent() : CComponent(){
	//컴포넌트 id
	m_ID = COMPONENTID_END;

	//같은 종류의 컴포넌트를 모아놓은 패밀리 id
	//ex) geometry 사용한 render와 그냥 render
	m_FamilyID = ANIMATE;

	//자신의 소유자
	m_pOwner = NULL;
}

CAnimateComponent::~CAnimateComponent() {

}