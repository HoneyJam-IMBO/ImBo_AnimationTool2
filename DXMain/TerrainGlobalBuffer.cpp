#include "stdafx.h"
#include "TerrainGlobalBuffer.h"

bool CTerrainGlobalBuffer::Begin(float fOneSpaceSize, float fOneSideSpaceNumRcp){
	//사용할 버퍼 만들고
	CConstantBuffer::Begin(1, sizeof(TERRAIN_GLOBAL_VALUE), 10, BIND_DS, 0);

	//사용할 정보 초기화
	m_value.OneSideSpaceNumRcp = fOneSideSpaceNumRcp;
	m_value.OneSpaceSize = fOneSpaceSize;


	return true;
}

bool CTerrainGlobalBuffer::End(){

	return true;
}

void CTerrainGlobalBuffer::UpdateShaderState(){
	void* pData = Map();

	TERRAIN_GLOBAL_VALUE * pGlobalValue = (TERRAIN_GLOBAL_VALUE *)pData;
	pGlobalValue = &m_value;

	Unmap();
}

void CTerrainGlobalBuffer::SetData(void * pData){
	TERRAIN_GLOBAL_VALUE * pGlobalValue = (TERRAIN_GLOBAL_VALUE *)pData;

	m_value.OneSideSpaceNumRcp = pGlobalValue->OneSideSpaceNumRcp;
	m_value.OneSpaceSize = pGlobalValue->OneSpaceSize;
}

CTerrainGlobalBuffer::CTerrainGlobalBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CConstantBuffer(pd3dDevice, pd3dDeviceContext){

}

CTerrainGlobalBuffer::~CTerrainGlobalBuffer()
{
}
