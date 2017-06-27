#include "stdafx.h"
#include "WeaponPart.h"

//----------------------------dxobject-----------------------------
bool CWeaponPart::Begin() {
	//m_partID = part_id::PART_WEAPON;
	m_objectID = object_id::OBJECT_WEAPON;

	return CPart::Begin();
}
bool CWeaponPart::End() {

	return CPart::End();
}
//----------------------------dxobject-----------------------------

CWeaponPart::CWeaponPart() {

}
CWeaponPart::~CWeaponPart() {

}