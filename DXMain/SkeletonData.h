#pragma once
#pragma once
#include "Object.h"
#include "FbxBlendWeightPair.h"
#include "FbxJointData.h"

//this is joint¿« ∏¿”!
class CSkeletonData : public CObject {
public:
	bool Begin();
	virtual bool End();

	UINT GetJointCnt() { return m_JointDatas.size(); }
	vector<CFbxJointData>& GetJointDatas() { return m_JointDatas; }

private:
	vector<CFbxJointData> m_JointDatas;

public:
	CSkeletonData() : CObject("skeletondata") {}
	~CSkeletonData() {}
};