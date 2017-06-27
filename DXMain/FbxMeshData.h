#pragma once
#include "Object.h"
#include "ControlPoint.h"
#include "VertexData.h"

class CFbxMeshData :public CObject {
public:
	//vector<CControlPoint>& GetControlPoints() { return m_vControlPoint; }
	//vector<XMFLOAT3>& GetNormals() { return m_vNormal; }
	//vector<XMFLOAT2>& GetUVs() { return m_vUV; }
	vector<CVertexData>& GetVertexDatas() { return m_vVertexData; }
	vector<UINT>& GetIndexs() { return m_vIndex; }
	vector<wstring>& GetTexturePaths() { return m_vTexturePath; }

	//UINT GetControlPointCnt() { return m_vControlPoint.size(); }
	//UINT GetNormalCnt() { return m_vNormal.size(); }
	//UINT GetUVCnt() { return m_vUV.size(); }
	UINT GetVertexCnt() { return static_cast<UINT>(m_vVertexData.size()); }
	UINT GetIndexCnt() { return static_cast<UINT>(m_vIndex.size()); }
	

	const char* GetMeshName() { return m_pMeshName; }
	bool GetHasNormal() { return m_hasNormal; }
	bool GetHasUV() { return m_hasUV; }
	bool GetHasT() { return m_hasT; }
	bool GetHasB() { return m_hasB; }
	bool GetByControlPoint() { return m_ByControlPoint; }
	void SetHasNormal(bool b) { m_hasNormal = b; }
	void SetHasUV(bool b) { m_hasUV = b; }
	void SetHasT(bool b) { m_hasT = b; }
	void SetHasB(bool b) { m_hasB = b; }
	void SetByControlPoint(bool b) { m_ByControlPoint = b; }
	void SetMeshName(const char* pName) { m_pMeshName = pName; }

	void SetAABB(CBoundingBox* pAABB) { m_pAABB = pAABB; }
	CBoundingBox* GetAABB() { return m_pAABB; }
private:
	CBoundingBox* m_pAABB{ nullptr };
	//mesh info
	const char* m_pMeshName{ nullptr };
	//position + weight blend value
	vector<CVertexData> m_vVertexData;
	vector<UINT> m_vIndex;
	vector<wstring> m_vTexturePath;
	//mesh info

	//exception data
	bool				m_hasNormal{ false };
	bool				m_hasUV{ false };
	bool				m_hasT{ false };
	bool				m_hasB{ false };
	bool				m_ByControlPoint{ false };

public:
	CFbxMeshData() : CObject("fbxmeshdata") {};
	~CFbxMeshData() {};
};