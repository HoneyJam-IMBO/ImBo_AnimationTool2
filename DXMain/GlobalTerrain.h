#pragma once
#include "GlobalObject.h"

//이놈은 전역 변수를 설정해 주기 위해..
//터레인이 탄생시킨 객체로써 전역 상수버퍼의 정보를 set하기 위한 객체이다.
//공간, 객체 정보등은 가지지 않지만 해당 객체의 전역 상수버퍼 데이터를 가지도록 한다.
//이 객체를 상속해서 필요한 정보를 set하고 setGlobalBufferinfo함수를 만들면 된다.
struct TERRAIN_GLOBAL_VALUE {
	float OneSpaceSizeRcp{ 0 };// 1/하나의 공간의 크기
	float OneSideSpaceNumRcp{ 0 };// 1/한의 사이드에 있는 공간 수 
	float OneSideSpaceNum{ 0 };//1 / 한의 사이드에 있는 공간 수 
	float HeightScale{ 0 };//높이값 scale
};

class CGlobalTerrain : public CGlobalObject {
public:
	//----------------------------object-----------------------------
	bool Begin(float fOneSpaceSize, float fOneSidSpaceNum, float fHeightScale);
	virtual bool End();
	//----------------------------object-----------------------------

	//--------------------------globalobject--------------------------
	//virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);
	virtual void SetGlobalBufferInfo(void** ppMappedResources);
	virtual void SetGlobalInfo(void* pData);
	//--------------------------globalobject--------------------------

protected:
	TERRAIN_GLOBAL_VALUE* m_pGlobalTerrainData;

public:
	CGlobalTerrain();
	virtual ~CGlobalTerrain();
};