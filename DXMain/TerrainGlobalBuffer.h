#pragma once
#include "ConstantBuffer.h"


struct TERRAIN_GLOBAL_VALUE {
	float OneSpaceSize{ 0 };//하나의 공간의 크기
	float OneSideSpaceNumRcp{ 0 };//1 / 한의 사이드에 있는 공간 수 
};

class CTerrainGlobalBuffer : public CConstantBuffer {
public:
	//object
	bool Begin(float fOneSpaceSize, float fOneSidSpaceNumRcp);
	virtual bool End();
	//object

	//---------------------globalbuffer-----------------------------------
	virtual void UpdateShaderState();
	virtual void SetData(void* pData);

private:
	//데이터 만들어 주고
	TERRAIN_GLOBAL_VALUE  m_value;


public:
	CTerrainGlobalBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CTerrainGlobalBuffer();

};