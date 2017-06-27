#include "stdafx.h"
#include "Object.h"


bool CObject::Begin() {
	m_name.clear();
	m_tag = tag::TAG_DEFAULT;

	return true;
}
bool CObject::End() {

	return true;
}
CObject::CObject(string name, tag t) : m_name(name), m_tag(t) {

}
CObject::~CObject() {

}