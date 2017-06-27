#include "stdafx.h"
#include "GlobalTerrain.h"


bool CGlobalTerrain::Begin(float fOneSpaceSize, float fOneSidSpaceNum, float fHeightScale) {
	m_objectID = object_id::OBJECT_TERRAIN;

	m_pGlobalTerrainData = new TERRAIN_GLOBAL_VALUE;
	m_pGlobalTerrainData->OneSideSpaceNum = fOneSidSpaceNum;
	m_pGlobalTerrainData->OneSideSpaceNumRcp = 1.0f/fOneSidSpaceNum;
	m_pGlobalTerrainData->OneSpaceSizeRcp = 1.0f/fOneSpaceSize;
	m_pGlobalTerrainData->HeightScale = fHeightScale;

	CGlobalObject::Begin();
	return true;
}
bool CGlobalTerrain::End() {
	if (m_pGlobalTerrainData) delete m_pGlobalTerrainData;
	return true;
}

//instance buffer controll base
void CGlobalTerrain::SetGlobalBufferInfo(void** ppMappedResources) {

	////형변환
	//VS_VB_INSTANCE *pnInstances = (VS_VB_INSTANCE *)ppMappedResources[0];
	////transpose 이후 정보 주입
	//pnInstances[nInstance].m_xmmtxWorld = XMMatrixTranspose(GetWorldMtx());

	TERRAIN_GLOBAL_VALUE* pTerrainGlobalData = (TERRAIN_GLOBAL_VALUE*)ppMappedResources[0];
	pTerrainGlobalData[0] = *m_pGlobalTerrainData;
}

void CGlobalTerrain::SetGlobalInfo(void * pData) {
	TERRAIN_GLOBAL_VALUE* pTerrainGlobalData = (TERRAIN_GLOBAL_VALUE*)pData;
	m_pGlobalTerrainData->OneSideSpaceNum = pTerrainGlobalData->OneSideSpaceNum;
	m_pGlobalTerrainData->OneSideSpaceNumRcp = pTerrainGlobalData->OneSideSpaceNumRcp;
	m_pGlobalTerrainData->OneSpaceSizeRcp = pTerrainGlobalData->OneSpaceSizeRcp;
	m_pGlobalTerrainData->HeightScale = pTerrainGlobalData->HeightScale;

	UpdateGlobalBuffer();
}

//생성자는 위에서부터 
CGlobalTerrain::CGlobalTerrain() : CGlobalObject("globalterrain") {

}
CGlobalTerrain ::~CGlobalTerrain() {

}