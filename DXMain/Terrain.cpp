#include "stdafx.h"
#include "Terrain.h"


bool CTerrain::Begin() {
	//object_id set
	m_objectID = object_id::OBJECT_TERRAIN;
	CGameObject::Begin();
	return true;
}
bool CTerrain::End() {

	return CGameObject::End();
}


CTerrain::CTerrain() : CGameObject("terrain", tag::TAG_STATIC_OBJECT) {

}
CTerrain::~CTerrain() {

}