#pragma once
#include "Object.h"

typedef pair<UINT, double> BlendWeightPair;
class CFbxBlendWeightPair : public CObject {
public:
	void GetBlendWeightPair(UINT& nIndex, double& Weight);
	UINT GetIndex() { return m_nIndex; }
	double GetWeight() { return m_Weight; }

	void SetBlendWeightPair(UINT nIndex, double Weight);
	void SetWeight(double Weight) { m_Weight = Weight; }
	void SetIndex(UINT index) { m_nIndex = index; }
private:
	UINT m_nIndex{ 0 };
	double m_Weight{ 0.0 };

public:
	CFbxBlendWeightPair(UINT nIndex, double Weight);
	~CFbxBlendWeightPair();
};