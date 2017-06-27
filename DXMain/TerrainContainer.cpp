#include "stdafx.h"
#include "TerrainContainer.h"
#include "SpaceContainer.h"


void CTerrainContainer::Begin(LPCTSTR pHeightmapFileName, int nWidth, int nLength, float fHeightScale, CSpaceContainer* pSpaceContainer) {
	m_pSpaceContainer = pSpaceContainer;

	//global object 제작
	m_pGlobalTerrain = new CGlobalTerrain();
	float fOneSpaceSize = (float)m_pSpaceContainer->GetOneSpaceSize();
	float fOneSideSpaceNum = (float)m_pSpaceContainer->GetOneSideSpaceNum();
	//global object set, Update
	m_pGlobalTerrain->Begin(fOneSpaceSize, fOneSideSpaceNum, fHeightScale);


	//터레인 제작
	//terrain
	CTerrain* pTerrain = nullptr;
	for (int j = 0; j < ONESIDE_SPACE_NUM; ++j) {
		for (int i = 0; i < ONESIDE_SPACE_NUM; ++i) {
			pTerrain = new CTerrain();
			pTerrain->Begin();
			//pTerrain->SetRenderContainer(pSeller);
			pTerrain->SetPosition(XMVectorSet(i*ONESPACE_SIZE, 0.f, j*ONESPACE_SIZE, 0.f));
			m_pSpaceContainer->AddObject(pTerrain);
		}
	}
	//terrain

	//height map data init
	m_nWidth = nWidth;
	m_nLength = nLength;
	m_xmf3Scale = XMFLOAT3(static_cast<float>(m_pSpaceContainer->GetSize() / (m_nWidth-1)),
		fHeightScale, static_cast<float>(m_pSpaceContainer->GetSize()/(m_nLength - 1)));

	BYTE *pHeightMapImage = new BYTE[m_nWidth * m_nLength];

	HANDLE hFile = ::CreateFile(pHeightmapFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
	DWORD dwBytesRead;
	::ReadFile(hFile, pHeightMapImage, (m_nWidth * m_nLength), &dwBytesRead, NULL);
	::CloseHandle(hFile);

	m_pHeightMapImage = new BYTE[m_nWidth * m_nLength];
	for (int y = 0; y < m_nLength; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_pHeightMapImage[x + ((m_nLength - 1 - y)*m_nWidth)] = pHeightMapImage[x + (y*m_nWidth)];
		}
	}

	if (pHeightMapImage) delete[] pHeightMapImage;
	//height map data init
}

bool CTerrainContainer::End() {
	if (m_pGlobalTerrain) delete m_pGlobalTerrain;

	return false;
}

float CTerrainContainer::GetHeight(XMVECTOR xmvPos){
	XMFLOAT3 xmf3Pos;
	XMStoreFloat3(&xmf3Pos, xmvPos);
	float fx = xmf3Pos.x;
	float fz = xmf3Pos.z;

	fx = fx / m_xmf3Scale.x;
	fz = fz / m_xmf3Scale.z;
	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= m_nWidth) || (fz >= m_nLength)) return(0.0f);

	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;

	float fBottomLeft = (float)m_pHeightMapImage[x + (z*m_nWidth)];
	float fBottomRight = (float)m_pHeightMapImage[(x + 1) + (z*m_nWidth)];
	float fTopLeft = (float)m_pHeightMapImage[x + ((z + 1)*m_nWidth)];
	float fTopRight = (float)m_pHeightMapImage[(x + 1) + ((z + 1)*m_nWidth)];

	if (fzPercent < (1.0f - fxPercent))
		fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
	else
		fBottomLeft = fTopLeft + (fBottomRight - fTopRight);

#ifdef _WITH_APPROXIMATE_OPPOSITE_CORNER
	if (bReverseQuad)
	{
		if (fzPercent >= fxPercent)
			fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
		else
			fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
	}
	else
	{
		if (fzPercent < (1.0f - fxPercent))
			fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
		else
			fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
	}
#endif
	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;

	return(fHeight * m_xmf3Scale.y);
	
}

CTerrainContainer::CTerrainContainer() : CObject("terraincontainer") {
}

CTerrainContainer::~CTerrainContainer()
{
}

