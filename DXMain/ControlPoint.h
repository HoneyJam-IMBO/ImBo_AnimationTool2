#pragma once
#include "Object.h"
#include "FbxBlendWeightPair.h"

class CControlPoint : public CObject {
public:
	XMFLOAT3& GetPosition() { return m_xmf3Position; }
	void SetPosition(XMFLOAT3 xmf3Pos) { m_xmf3Position = xmf3Pos; }
	vector<CFbxBlendWeightPair>& GetBlendWeightPairs(){ return m_vBlendWeightPair; }

private:
	XMFLOAT3 m_xmf3Position;
	vector<CFbxBlendWeightPair> m_vBlendWeightPair;

public:
	CControlPoint(XMFLOAT3& xmf3Pos) : CObject("controlpoint") { m_xmf3Position = xmf3Pos; };
	~CControlPoint() {};
};