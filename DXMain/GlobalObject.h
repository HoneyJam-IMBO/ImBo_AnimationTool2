#pragma once
#pragma once
#include "Object.h"

//class CRenderContainerSeller;
class CRenderContainer;
//이놈은 전역 변수를 설정해 주기 위해..
//터레인이 탄생시킨 객체로써 전역 상수버퍼의 정보를 set하기 위한 객체이다.
//공간, 객체 정보등은 가지지 않지만 해당 객체의 전역 상수버퍼 데이터를 가지도록 한다.
//이 객체를 상속해서 필요한 정보를 set하고 setGlobalBufferinfo함수를 만들면 된다.
//간단히 global buffer data를 관리하는객체
class CGlobalObject : public CObject {
public:
	//----------------------------object-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	//--------------------------globalobject--------------------------
	virtual void SetGlobalBufferInfo(void** ppMappedResources);
	virtual void SetDebugGlobalBufferInfo(void ** ppMappedResources);
	virtual void SetGlobalInfo(void* pData);
	//--------------------------globalobject--------------------------

	void UpdateGlobalBuffer();
	

protected:
	//object_id
	object_id m_objectID{ object_id::OBJECT_END };
	CRenderContainer* m_pRenderContainer{ nullptr };

public:
	CGlobalObject(string name, tag t = tag::TAG_DEFAULT);
	virtual ~CGlobalObject();
};