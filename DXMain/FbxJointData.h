#pragma once
#include "Object.h"
#include "KeyFrame.h"
#include "BoundingBox.h"

class CFbxJointData : public CGameObject {
public:
	void SetJointName(string name) { m_sName = name; }
	void SetParentIndex(UINT index) { m_ParentIndex = index; }
	void SetMyIndex(UINT index) { m_MyIndex = index; }
	void SetOffsetMtx(XMMATRIX& mtx) { m_xmmtxOffset = mtx; }
	
	string GetJointName() { return m_sName; }
	UINT GetParentIndex() { return m_ParentIndex; }
	UINT GetMyIndex() { return m_MyIndex; }
	XMMATRIX& GetOffsetMtx() { return m_xmmtxOffset; }

	bool operator==(CFbxJointData& data) {
		return m_sName == data.m_sName;
	}
	bool operator!=(CFbxJointData& data) {
		return m_sName != data.m_sName;
	}

private:
	string m_sName;
	UINT m_MyIndex{ 0 };
	UINT m_ParentIndex{ 0 };
	XMMATRIX m_xmmtxOffset;

public:
	CFbxJointData() :CGameObject("fbxjointdata") {
	};
	~CFbxJointData() {}
};
