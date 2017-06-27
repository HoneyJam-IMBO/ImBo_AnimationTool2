#include "stdafx.h"
#include "BoostPart.h"

//----------------------------dxobject-----------------------------
bool CBoostPart::Begin() {
	//m_partID = part_id::PART_BOOST;
	m_objectID = object_id::OBJECT_BOOST;
	m_fVelocity = 10;
	m_fHP = 100;
	m_fSize = 8;

	return CPart::Begin();
}
bool CBoostPart::End() {

	return CPart::End();
}
//----------------------------dxobject-----------------------------

CBoostPart::CBoostPart() : CFunctionPart() {

}
CBoostPart::~CBoostPart() {

}