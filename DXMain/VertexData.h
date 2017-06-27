#pragma once
#include "Object.h"
#include "FbxBlendWeightPair.h"


class CVertexData : public CObject {
public:
	XMFLOAT3& GetPosition() { return m_xmf3Position; }
	void SetPosition(XMFLOAT3& xmf3Pos) { m_xmf3Position = xmf3Pos; }
	XMFLOAT3& GetNormal() { return m_xmf3Normal; }
	void SetNormal(XMFLOAT3& xmf3Normal) { m_xmf3Normal = xmf3Normal; }
	XMFLOAT2& GetUV() { return m_xmf2UV; }
	void SetUV(XMFLOAT2& xmf2UV) { m_xmf2UV = xmf2UV; }
	XMFLOAT3& GetT() { return m_xmf3T; }
	void SetT(XMFLOAT3& xmf3T) { m_xmf3T = xmf3T; }
	XMFLOAT3& GetB() { return m_xmf3B; }
	void SetB(XMFLOAT3& xmf3B) { m_xmf3B = xmf3B; }
	vector<CFbxBlendWeightPair>& GetBlendWeightPairs() { return m_vBlendWeightPair; }

	BOOL operator == (CONST CVertexData& others) const
	{
		float diff_P_X = 0, diff_P_Y = 0, diff_P_Z = 0;
		float diff_N_X = 0, diff_N_Y = 0, diff_N_Z = 0;
		float diff_T_X = 0, diff_T_Y = 0;

		diff_P_X = this->m_xmf3Position.x - others.m_xmf3Position.x;
		diff_P_Y = this->m_xmf3Position.y - others.m_xmf3Position.y;
		diff_P_Z = this->m_xmf3Position.z - others.m_xmf3Position.z;

		diff_N_X = this->m_xmf3Normal.x - others.m_xmf3Normal.x;
		diff_N_Y = this->m_xmf3Normal.y - others.m_xmf3Normal.y;
		diff_N_Z = this->m_xmf3Normal.z - others.m_xmf3Normal.z;

		diff_T_X = this->m_xmf2UV.x - others.m_xmf2UV.x;
		diff_T_Y = this->m_xmf2UV.y - others.m_xmf2UV.y;


		bool comparePos = (fabs(diff_P_X) <= 0.0001) && (fabs(diff_P_Y) <= 0.0001) && (fabs(diff_P_Z) <= 0.0001);
		bool compareNormal = (fabs(diff_N_X) <= 0.0001) && (fabs(diff_N_Y) <= 0.0001) && (fabs(diff_N_Z) <= 0.0001);
		bool compareUV = (fabs(diff_T_X) <= 0.0001) && (fabs(diff_T_Y) <= 0.0001);

		if (comparePos && compareNormal && compareUV)
			return true;
		else
			return false;
	}
private:
	XMFLOAT3 m_xmf3Position;
	XMFLOAT3 m_xmf3Normal;
	XMFLOAT2 m_xmf2UV;
	XMFLOAT3 m_xmf3T;
	XMFLOAT3 m_xmf3B;
	vector<CFbxBlendWeightPair> m_vBlendWeightPair;

public:
	CVertexData(XMFLOAT3& xmf3Pos, XMFLOAT3& xmf3Normal, XMFLOAT2& xmf2UV) : 
		m_xmf3Position(xmf3Pos), m_xmf3Normal(xmf3Normal), m_xmf2UV(xmf2UV), CObject("vertexdata") { };
	CVertexData() :
		CObject("vertexdata") { };

	~CVertexData() {};
};