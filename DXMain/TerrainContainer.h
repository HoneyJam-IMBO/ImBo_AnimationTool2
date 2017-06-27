#pragma once
#include "Object.h"
#include "Terrain.h"
//#include "RenderContainerSeller.h"
#include "GlobalTerrain.h"

class CSpaceContainer;

class CTerrainContainer : public CObject {
public:
	void Begin(LPCTSTR pHeightmapFileName, int nWidth, int nLength, float HeightScale, CSpaceContainer* pSpaceContainer);
	bool End();

	float GetHeight(XMVECTOR xmvPos);
private:
	CSpaceContainer* m_pSpaceContainer{ nullptr };
	CGlobalTerrain* m_pGlobalTerrain{ nullptr };
	CRenderContainer* m_pTerrainRenderContainer{ nullptr };

	int m_nWidth{ 0 };
	int m_nLength{ 0 };
	BYTE* m_pHeightMapImage{ nullptr };
	XMFLOAT3 m_xmf3Scale;
public:
	CTerrainContainer();
	~CTerrainContainer();
};