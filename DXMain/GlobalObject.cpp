#include "stdafx.h"
#include "GlobalObject.h"
//#include "RenderContainerSeller.h"
#include "RenderContainer.h"

bool CGlobalObject::Begin(){
	m_pRenderContainer = RCSELLER->GetRenderContainer(m_objectID);
	m_pRenderContainer->SetGlobalObject(this);

	UpdateGlobalBuffer();

	return true;
}

bool CGlobalObject::End(){

	return false;
}

void CGlobalObject::SetGlobalBufferInfo(void ** ppMappedResources){

}
void CGlobalObject::SetDebugGlobalBufferInfo(void ** ppMappedResources) {

}
void CGlobalObject::SetGlobalInfo(void * pData){

}


void CGlobalObject::UpdateGlobalBuffer(){
	m_pRenderContainer->UpdateGlobalBuffer();
}

CGlobalObject::CGlobalObject(string name, tag t) : CObject(name, t){
	
}

CGlobalObject::~CGlobalObject(){

}
