#pragma once
#include "DXObject.h"
#include "AnimationData.h"
#include "StaticBuffer.h"
#include "BoundingBox.h"

class CAnimater;

class CAnimationInfo :public DXObject {
public:
	bool Begin(shared_ptr<CAnimater> pAnimater);
	virtual bool End();
	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void Update(float fTimeElapsed);

	float& GetCurFrame() { return m_CurFrame; }
	int& GetFrameCnt() { return m_pAnimationData->GetAnimationLength(); }
	bool& GetbAnimation() { return m_bAnimation; }
	float& GetAnimationSpd() { return m_fAnimationSpd; }
	UINT GetAnimationIndex() { return m_AnimationIndex; }
	void SetAnimationIndex(UINT index) { m_AnimationIndex = index; }
	void SetAnimater(shared_ptr<CAnimater> pAnimater) { m_pAnimater = pAnimater; }
	void SetAnimationData(CAnimationData* pAnimationData) { m_pAnimationData = pAnimationData; }
	//utill 
	void Reset();

	//joint data 수정 함수
	//void ChangeJointData(vector<string>& vJointName);

	XMMATRIX GetCurFrameMtx(UINT JointIndex, UINT meshIndex = 0) { return m_pAnimationData->GetKeyFrames(JointIndex)[m_CurFrame].GetKeyFrameTransformMtx(); };
	CAnimationData* GetAnimationData() { return m_pAnimationData; }
	//map<UINT, vector<CFbxJointData>>& GetAnimationInfos() { return m_mMeshIndexJoints; }
	//ui proc
	void SelectAnimationProc();
	void DeleteActiveJointProc();

	void ClearSelectOBBUI();
	void SetRHand();
	void SetLHand();
	void SetHead();
	void CreateSelectOBBUI();
	void CreateActiveOBBUI();
	void CreateAnimationInfoUI();

	vector<CBoundingBox>& GetTempOBB() { return m_vTempBoundingBox; }
	list<CBoundingBox*>& GetActiveOBB() { return m_lActiveBoundingBox; }

	shared_ptr<CAnimater> GetAnimater() { return m_pAnimater; }

	//create func
	static CAnimationInfo* CreateAnimationInfoFromFBXFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, shared_ptr<CAnimater>  pAnimater);
	static CAnimationInfo* CreateAnimationInfoFromGJMFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, shared_ptr<CAnimater>  pAnimater);

	void SetAnimationSpd(float spd) { m_fAnimationSpd = spd; }

	vector<BoundingOrientedBox>& GetActiveBoundingBox() { return m_vActiveBoundingBox; }
	vector<XMMATRIX>& GetSelectJoint() { return m_vSelectJointCoord; }
private:
	vector<CBoundingBox> m_vTempBoundingBox;
	list<CBoundingBox*> m_lActiveBoundingBox;
	vector<BoundingOrientedBox> m_vActiveBoundingBox;
	vector<XMMATRIX> m_vSelectJointCoord;
	float m_fAnimationSpd{ 30.0f };
	//animation 
	CAnimationData* m_pAnimationData{ nullptr };

	UINT m_AnimationIndex{ 0 };//x


	//buffer
	CStaticBuffer* m_pAnimBuffer{ nullptr };//x

	//map<UINT, vector<CFbxJointData>> m_mMeshIndexJoints;
	float m_CurFrame{ 0 };//x
	bool m_bAnimation{ true };//x
	//map<int, int> m_mChangeIndex;
	shared_ptr<CAnimater> m_pAnimater{ nullptr };//x
public:
	CAnimationInfo(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CAnimationInfo();

};