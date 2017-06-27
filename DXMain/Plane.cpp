#include "stdafx.h"
#include "Plane.h"

bool CPlane::Begin() {
	//object_id set
	m_objectID = object_id::OBJECT_PLANE;
	CGameObject::Begin();
	//새로운 객체가 생겨나면 Begin에서 Component들을 set해준다. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);

	return true;
}
bool CPlane::End() {

	return CGameObject::End();
}


CPlane::CPlane() : CGameObject("plane", tag::TAG_STATIC_OBJECT) {

}
CPlane::~CPlane() {

}