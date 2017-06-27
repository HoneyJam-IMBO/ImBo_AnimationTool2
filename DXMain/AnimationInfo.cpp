#include "stdafx.h"
#include "AnimationInfo.h"
#include "Animater.h"

void TW_CALL AnimationOnOffButtonCallback(void * clientData) {
	bool* pBool = reinterpret_cast<bool*>(clientData);
	if (*pBool) {
		*pBool = false;
	}
	else {
		*pBool = true;
	}
}

void TW_CALL AnimationResetButtonCallback(void * clientData) {
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimationInfo->GetCurFrame() = 0;
}
void TW_CALL AnimationDeleteButtonCallback(void * clientData) {
	//나중에 수정해야 한다.
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimationInfo->GetAnimater()->DeleteAnimationInfo(pAnimationInfo->GetAnimationIndex());

	TWBARMGR->DeleteBar("AnimationInfo");
	TWBARMGR->DeleteBar("ActiveJoint");
}
void TW_CALL DeleteOBBButtonCallback(void * clientData) {
	
	CBoundingBox* pBoundingBox = reinterpret_cast<CBoundingBox*>(clientData);
	char deleteGroupName[64];
	sprintf(deleteGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());

	pBoundingBox->SetToolActive(false);

	//TWBARMGR->DeleteVar("Active Joint", deleteGroupName);
}

void TW_CALL CreateSelectOBBCallback(void * clientData) {
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimationInfo->CreateSelectOBBUI();
}
void TW_CALL CreateClearOBBCallback(void * clientData) {
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimationInfo->ClearSelectOBBUI();
}
void TW_CALL CreateSetRHandCallback(void * clientData) {
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimationInfo->SetRHand();
}
void TW_CALL CreateSetLHandCallback(void * clientData) {
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimationInfo->SetLHand();
}
void TW_CALL CreateSetHeadCallback(void * clientData) {
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimationInfo->SetHead();
}
bool CAnimationInfo::Begin(shared_ptr<CAnimater> pAnimater){
	int nJoint{ 0 };

	nJoint = m_pAnimater->GetSkeletonData()->GetJointDatas().size();

	for (int j = 0; j < nJoint; ++j) {
		XMMATRIX FrameTransform;
		CBoundingBox boundingBox;
		boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
		boundingBox.SetActive(false);
		m_vTempBoundingBox.push_back(boundingBox);
	}

//	for (int MeshIndex = 0; MeshIndex < FBXIMPORTER->GetAnimationDatas().size(); ++MeshIndex) {
//		nJoint = static_cast<int>(FBXIMPORTER->GetAnimationDatas()[MeshIndex].GetJointCnt());
//
//		for (int j = 0; j < nJoint; ++j) {
//			XMMATRIX FrameTransform;
//			m_mMeshIndexJoints[MeshIndex].push_back(FBXIMPORTER->GetAnimationDatas()[MeshIndex].GetJointDatas()[j]);
//			CBoundingBox boundingBox;
//			boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
//			boundingBox.SetActive(false);
//			m_vTempBoundingBox.push_back(boundingBox);
//		}
//	}
	m_pAnimBuffer = new CStaticBuffer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pAnimBuffer->Begin(256, sizeof(XMMATRIX), nullptr, 10, BIND_VS);

	m_pAnimater->AddAnimationInfo(this);
	return true;
}

bool CAnimationInfo::End(){
	m_pAnimationData->End();
	delete m_pAnimationData;

	m_pAnimBuffer->End();
	delete m_pAnimBuffer;

	for (auto data : m_lActiveBoundingBox) {
		data->End();
		delete data;
	}
	//int MeshNum = m_mMeshIndexJoints.size();
	//for (int i = 0; i<MeshNum; ++i) {
	//	m_mMeshIndexJoints[i].clear();
	//}
	//m_mMeshIndexJoints.clear();

	return true;

}

void CAnimationInfo::SetShaderState(){
	void* pData = m_pAnimBuffer->Map();
	XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);

	for (int j = 0; j < m_pAnimater->GetSkeletonData()->GetJointDatas().size(); ++j) {
		
		//pAnimationData[j] = XMMatrixIdentity();
		if (m_pAnimationData->GetKeyFrames(j).empty()) {
			continue;
		}
		XMMATRIX offsetMtx = m_pAnimater->GetSkeletonData()->GetJointDatas()[j].GetOffsetMtx();
		pAnimationData[j] = XMMatrixTranspose(offsetMtx * m_pAnimationData->GetKeyFrames(j)[m_CurFrame].GetKeyFrameTransformMtx());
		//		if (m_vOBB[j].GetActive()) {
		//	
		//			DEBUGER->RegistCoordinateSys(m_ppAnimationData[static_cast<int>(m_nFrame)][j]);
		//			BoundingOrientedBox originObb = m_vOBB[j].GetOBB();
		//			originObb.Transform(originObb, m_ppAnimationData[static_cast<int>(m_nFrame)][j]);
		//			DEBUGER->RegistOBB(originObb);
		//		}
	}
	m_pAnimBuffer->Unmap();
	m_pAnimBuffer->SetShaderState();
}

void CAnimationInfo::CleanShaderState(){
	m_pAnimBuffer->CleanShaderState();
}

void CAnimationInfo::Update(float fTimeElapsed){
	vector<CBoundingBox*> vDeleteBoundingBox;
	int DeleteBoxCnt{ 0 };
	//debug
	m_vSelectJointCoord.clear();
	for (int tempOBBCnt = 0; tempOBBCnt < m_vTempBoundingBox.size(); ++tempOBBCnt) {
		if (m_vTempBoundingBox[tempOBBCnt].GetActive()) {
			if(false == m_pAnimationData->GetKeyFrames(tempOBBCnt).empty())
				m_vSelectJointCoord.push_back(m_pAnimationData->GetKeyFrames(tempOBBCnt)[m_CurFrame].GetKeyFrameTransformMtx());
		}
	}
	//debug
	m_vActiveBoundingBox.clear();
	for (auto data : m_lActiveBoundingBox) {
		float fMin = data->GetMin();
		float fMax = data->GetMax();
		if (fMin <= m_CurFrame && m_CurFrame <= fMax) data->SetActive(true);
		else data->SetActive(false);

		if (data->GetToolActive()) {//ui 삭제 로직 
			
			if (data->GetActive()) {
				BoundingOrientedBox originObb = data->GetOBB();
				if (false == m_pAnimationData->GetKeyFrames(data->GetMyJointIndex()).empty()) {
					originObb.Transform(originObb, m_pAnimationData->GetKeyFrames(data->GetMyJointIndex())[m_CurFrame].GetKeyFrameTransformMtx());
					m_vActiveBoundingBox.push_back(originObb);
					//originObb.Transform(originObb, data->GetWorldMtx());
					//DEBUGER->RegistOBB(originObb);
				}
			}
		}
		else {
			vDeleteBoundingBox.push_back(data);
		}
	}
	if (false == vDeleteBoundingBox.empty()) {
		for (auto data : vDeleteBoundingBox) {
			m_lActiveBoundingBox.remove(data);
		}
		DeleteActiveJointProc();
	}
	//update animation data
	if (m_bAnimation) {
		m_CurFrame = m_CurFrame + (fTimeElapsed*m_fAnimationSpd);
	}
	if (m_CurFrame > m_pAnimationData->GetAnimationLength()) {
		m_CurFrame = 0.f;
	}
}

void CAnimationInfo::Reset(){
	m_CurFrame = 0;
}

//void CAnimationInfo::ChangeJointData(vector<string>& vJointName){
//	//인자로 들어온 animater의 joint tree정보를 가지고 
//	//자신의 joint데이터를 갱신한다 . 
//	//인자로 들어온 joint tree정보는 모든 joint들의 정보를 아우른다.
//	
//	//1. 새로운 벡터를 제작한다.
//	vector<CFbxJointData> vTempJoints;
//	vector<CFbxJointData> ::iterator iter;
//	int CurIndex{ 0 };
//	int ChangeIndex{ 0 };
//	for (auto jointName : vJointName) {
//		CurIndex = 0;
//		//2. 인자로 들어온 animation tree만큼 루프를 돈다.
//		iter = find_if(m_vJoints.begin(), m_vJoints.end(), [&jointName, &CurIndex](CFbxJointData& my) {
//			CurIndex++;
//			return (my.GetJointName() == jointName);
//		});
//		//3. 각 joint의 이름을 가지고 기존의 joint의 이름과 비교하여 있으면 넣고
//		if (m_vJoints.end() != iter) {
//			vTempJoints.push_back(*iter);
//			m_mChangeIndex.insert(pair<int, int>(CurIndex, ChangeIndex));
//		}
//		//아니면 새로운 joint에게 offsetmtx만 넣고 frame mtx등 필요없는 데이터는 Identity를 넣는다.
//		else {//같은 이름이 없는 경우가 2가지 있다. 나에게는 있는데 상대에게는 없는 joint
//			//상대에게는 있는데 나에게 없는 joint가 그것이다.
//			//이경우는 상대에게는 있는데 나에겐 없는 joint경우만 해당한다. 
//			CFbxJointData data;
//			//data.SetMyIndex(joint.GetMyIndex());
//			//data.SetJointName(joint.GetJointName());
//			//data.SetOffsetMtx(joint.GetOffsetMtx());
//			//data.SetParentIndex(joint.GetParentIndex());
//			vTempJoints.push_back(data);
//		}
//		ChangeIndex++;
//	}
//	//4. 기존의 vector를 날리고 새로운 vector를 저장한다.
//	m_vJoints.clear();
//	m_vTempBoundingBox.clear();
//
//	for (auto data : vTempJoints) {
//		m_vJoints.push_back(data);
//		//bounding box 추가
//		CBoundingBox boundingBox;
//		boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
//		boundingBox.SetActive(false);
//		m_vTempBoundingBox.push_back(boundingBox);
//	}
//}

void CAnimationInfo::SelectAnimationProc(){
	CreateAnimationInfoUI();
	CreateActiveOBBUI();
}

void CAnimationInfo::DeleteActiveJointProc(){
	CreateActiveOBBUI();
}
void CAnimationInfo::ClearSelectOBBUI() {
	for (int i = 0; i < m_vTempBoundingBox.size(); ++i) {
		m_vTempBoundingBox[i].SetActive(false);
	}
	CreateAnimationInfoUI();
}
void CAnimationInfo::SetRHand(){
	for (int i = 0; i < m_vTempBoundingBox.size(); ++i) {
		if (m_vTempBoundingBox[i].GetActive()) {
			m_pAnimater->SetRHand(i);
			break;
		}
	}
	ClearSelectOBBUI();
}
void CAnimationInfo::SetLHand(){
	for (int i = 0; i < m_vTempBoundingBox.size(); ++i) {
		if (m_vTempBoundingBox[i].GetActive()) {
			m_pAnimater->SetLHand(i);
			break;
		}
	}
	ClearSelectOBBUI();
}
void CAnimationInfo::SetHead(){
	for (int i = 0; i < m_vTempBoundingBox.size(); ++i) {
		if (m_vTempBoundingBox[i].GetActive()) {
			m_pAnimater->SetHead(i);
			break;
		}
	}
	ClearSelectOBBUI();
}
void CAnimationInfo::CreateSelectOBBUI(){
	float max = GetFrameCnt();
	char* barName = "ActiveJoint";
	int TempOBBCnt{ 0 };
	
	TWBARMGR->DeleteBar(barName);
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 250, 700);
	TWBARMGR->SetBarPosition(barName, 750, 200);
	TWBARMGR->SetBarColor(barName, 200, 200, 0);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param
	for (auto ActiveOBB : m_lActiveBoundingBox) {
		ActiveOBB->SetMyIndex(TempOBBCnt++);
		//add obb position bar
		char GroupName[64];
		//sprintf(positionGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());
		sprintf(GroupName, "%s%d", m_pAnimater->GetSkeletonData()->GetJointDatas()[ActiveOBB->GetMyJointIndex()].GetJointName().c_str(), ActiveOBB->GetMyIndex());
		char positionMenuName[64];
		sprintf(positionMenuName, "%s %s", GroupName, "Position");
		TWBARMGR->AddSeparator(barName, GroupName, nullptr);
		TWBARMGR->AddPositionBar(barName, GroupName, positionMenuName, ActiveOBB,
			-100.f, 100.f, 0.1f);
		//add obb scale bar
		char scaleMenuName[64];
		sprintf(scaleMenuName, "%s %s", GroupName, "Scale");
		TWBARMGR->AddSeparator(barName, GroupName, nullptr);
		TWBARMGR->AddScaleBar(barName, GroupName, scaleMenuName, ActiveOBB,
			0.1f, 100.f, 0.1f);

		//add min max bar
		TWBARMGR->AddSeparator(barName, GroupName, nullptr);

		char minmaxMenuName[64];
		sprintf(minmaxMenuName, "%s%s", GroupName, " Min");
		TWBARMGR->AddMinMaxBarRW(barName, GroupName, minmaxMenuName, &ActiveOBB->GetMin(),
			0.1f, GetFrameCnt(), 0.1f);
		sprintf(minmaxMenuName, "%s%s", GroupName, " Max");
		TWBARMGR->AddMinMaxBarRW(barName, GroupName, minmaxMenuName, &ActiveOBB->GetMax(),
			0.1f, GetFrameCnt(), 0.1f);

		//add obb delete button
		char deleteMenuName[64];
		sprintf(deleteMenuName, "%s %s", GroupName, "Delete");
		TWBARMGR->AddSeparator(barName, GroupName, nullptr);
		TWBARMGR->AddButtonCB(barName, GroupName, deleteMenuName, DeleteOBBButtonCallback, ActiveOBB);
	}

	for (int i = 0; i < m_pAnimater->GetSkeletonData()->GetJointCnt(); ++i) {
		if (GetTempOBB()[i].GetActive()) {
			CBoundingBox* pBoundingBox = new CBoundingBox();
			pBoundingBox->Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
			//pBoundingBox->SetMyMeshIndex(j);
			pBoundingBox->SetMyJointIndex(i);
			pBoundingBox->SetMyIndex(GetActiveOBB().size());
			pBoundingBox->SetMax(max);
			m_lActiveBoundingBox.push_back(pBoundingBox);

			//add obb position bar
			char GroupName[64];
			//sprintf(positionGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());
			sprintf(GroupName, "%s%d", m_pAnimater->GetSkeletonData()->GetJointDatas()[i].GetJointName().c_str(), pBoundingBox->GetMyIndex());
			char positionMenuName[64];
			sprintf(positionMenuName, "%s %s", GroupName, "Position");
			TWBARMGR->AddSeparator(barName, GroupName, nullptr);
			TWBARMGR->AddPositionBar(barName, GroupName, positionMenuName, pBoundingBox,
				-100.f, 100.f, 0.1f);
			//add obb scale bar
			char scaleMenuName[64];
			sprintf(scaleMenuName, "%s %s", GroupName, "Scale");
			TWBARMGR->AddSeparator(barName, GroupName, nullptr);
			TWBARMGR->AddScaleBar(barName, GroupName, scaleMenuName, pBoundingBox,
				0.1f, 100.f, 0.1f);

			//add min max bar
			TWBARMGR->AddSeparator(barName, GroupName, nullptr);

			char minmaxMenuName[64];
			sprintf(minmaxMenuName, "%s%s", GroupName, " Min");
			TWBARMGR->AddMinMaxBarRW(barName, GroupName, minmaxMenuName, &pBoundingBox->GetMin(),
				0.1f, GetFrameCnt(), 0.1f);
			sprintf(minmaxMenuName, "%s%s", GroupName, " Max");
			TWBARMGR->AddMinMaxBarRW(barName, GroupName, minmaxMenuName, &pBoundingBox->GetMax(),
				0.1f, GetFrameCnt(), 0.1f);

			//add obb delete button
			char deleteMenuName[64];
			sprintf(deleteMenuName, "%s %s", GroupName, "Delete");
			TWBARMGR->AddSeparator(barName, GroupName, nullptr);
			TWBARMGR->AddButtonCB(barName, GroupName, deleteMenuName, DeleteOBBButtonCallback, pBoundingBox);
		}
	}
}

void CAnimationInfo::CreateActiveOBBUI(){
	float max = GetFrameCnt();
	char* barName = "ActiveJoint";
	
	TWBARMGR->DeleteBar(barName);
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 250, 700);
	TWBARMGR->SetBarPosition(barName, 750, 200);
	TWBARMGR->SetBarColor(barName, 200, 200, 0);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param
	int TempOBBCnt{ 0 };
	for (auto ActiveOBB : m_lActiveBoundingBox) {
		ActiveOBB->SetMyIndex(TempOBBCnt++);
		//add obb position bar
		char GroupName[64];
		//sprintf(positionGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());
		sprintf(GroupName, "%s%d", m_pAnimater->GetSkeletonData()->GetJointDatas()[ActiveOBB->GetMyJointIndex()].GetJointName().c_str(), ActiveOBB->GetMyIndex());
		char positionMenuName[64];
		sprintf(positionMenuName, "%s %s", GroupName, "Position");
		TWBARMGR->AddSeparator(barName, GroupName, nullptr);
		TWBARMGR->AddPositionBar(barName, GroupName, positionMenuName, ActiveOBB,
			-100.f, 100.f, 0.1f);
		//add obb scale bar
		char scaleMenuName[64];
		sprintf(scaleMenuName, "%s %s", GroupName, "Scale");
		TWBARMGR->AddSeparator(barName, GroupName, nullptr);
		TWBARMGR->AddScaleBar(barName, GroupName, scaleMenuName, ActiveOBB,
			0.1f, 100.f, 0.1f);

		//add min max bar
		TWBARMGR->AddSeparator(barName, GroupName, nullptr);

		char minmaxMenuName[64];
		sprintf(minmaxMenuName, "%s%s", GroupName, " Min");
		TWBARMGR->AddMinMaxBarRW(barName, GroupName, minmaxMenuName, &ActiveOBB->GetMin(),
			0.1f, GetFrameCnt(), 0.1f);
		sprintf(minmaxMenuName, "%s%s", GroupName, " Max");
		TWBARMGR->AddMinMaxBarRW(barName, GroupName, minmaxMenuName, &ActiveOBB->GetMax(),
			0.1f, GetFrameCnt(), 0.1f);

		//add obb delete button
		char deleteMenuName[64];
		sprintf(deleteMenuName, "%s %s", GroupName, "Delete");
		TWBARMGR->AddSeparator(barName, GroupName, nullptr);
		TWBARMGR->AddButtonCB(barName, GroupName, deleteMenuName, DeleteOBBButtonCallback, ActiveOBB);
	}
}

void CAnimationInfo::CreateAnimationInfoUI(){
	char barName[64];
	m_pAnimater->SetCurAnimationIndex(m_AnimationIndex);
	//m_AnimationName.c_str();
	sprintf(barName, "%s", "AnimationInfo");
	TWBARMGR->DeleteBar(barName);

	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 250, 300);
	TWBARMGR->SetBarPosition(barName, 0, 400);
	TWBARMGR->SetBarColor(barName, 200, 200, 0);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param

	TWBARMGR->AddMinMaxBarRW(barName, "Animation Info", "Animation Spd", &m_fAnimationSpd, 1.0f, 1000.f, 0.1f);
	TWBARMGR->AddMinMaxBarRW(barName, "Animation Info", "Animation Frame", &m_CurFrame, 0.f, m_pAnimationData->GetAnimationLength() - 1, 1.0f);
	TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation On/Off", AnimationOnOffButtonCallback, &m_bAnimation);
	TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation Reset", AnimationResetButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation Delete", AnimationDeleteButtonCallback, this);

	//create active obb button
	TWBARMGR->AddButtonCB(barName, "Button", "Create Select OBB", CreateSelectOBBCallback, this);
	TWBARMGR->AddButtonCB(barName, "Button", "Clear Select OBB", CreateClearOBBCallback, this);
	TWBARMGR->AddButtonCB(barName, "Button", "Set RHand", CreateSetRHandCallback, this);
	TWBARMGR->AddButtonCB(barName, "Button", "Set LHand", CreateSetLHandCallback, this);
	TWBARMGR->AddButtonCB(barName, "Button", "Set Haed", CreateSetHeadCallback, this);

	//모든 메쉬의 좌표계 ui
	int JointIndex{ 0 };
	//for (int i = 0; i < m_mMeshIndexJoints.size(); ++i) {//모든 메쉬의
	char groupName[64];
	sprintf(groupName, "%s%d %s", "Animation", m_AnimationIndex, "info");

	TWBARMGR->AddSeparator(barName, groupName, nullptr);
	for (int j = 0; j < m_pAnimater->GetSkeletonData()->GetJointDatas().size(); ++j) {//모든 joint
												//add active bool bar
		char menuName[64];
		sprintf(menuName, "%s", m_pAnimater->GetSkeletonData()->GetJointDatas()[j].GetJointName().c_str());

		TWBARMGR->AddBoolBar(barName, groupName, menuName, &m_vTempBoundingBox[JointIndex++].GetActive());
		//TWBARMGR->AddButtonCB("Animation1", groupName, menuName, JointSelectButtonCallback, &pMesh->GetOBBObject(j));
		//TWBARMGR->AddBoundingBoxActiveBar("Animation1", groupName, menuName, &pMesh->GetvJoint()[j]);

		//TWBARMGR->AddBoundingBoxActiveBar("PickingBar", groupName, boolMenuName, NULL);

	}
}

CAnimationInfo* CAnimationInfo::CreateAnimationInfoFromFBXFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, shared_ptr<CAnimater>  pAnimater){
	CAnimationInfo* pAnimationInfo = new CAnimationInfo(pd3dDevice, pd3dDeviceContext);
	pAnimationInfo->SetAnimationIndex(pAnimater->GetAnimationCnt());
	pAnimationInfo->SetAnimater(pAnimater);

	CAnimationData* pAnimationData = new CAnimationData();
	*pAnimationData = FBXIMPORTER->GetAnimationData();

	pAnimationInfo->SetAnimationData(pAnimationData);
	pAnimationInfo->Begin(pAnimater);
	return pAnimationInfo;
}

CAnimationInfo* CAnimationInfo::CreateAnimationInfoFromGJMFile(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, shared_ptr<CAnimater>  pAnimater){
	CAnimationInfo* pAnimationInfo = new CAnimationInfo(pd3dDevice, pd3dDeviceContext);
	pAnimationInfo->SetAnimationIndex(pAnimater->GetAnimationCnt());
	pAnimationInfo->SetAnimater(pAnimater);

	//obb info
	int obbCnt = IMPORTER->ReadInt();
	for (int j = 0; j < obbCnt; ++j) {
		CBoundingBox* pBoundingBox = new CBoundingBox();
		float min = IMPORTER->ReadFloat();
		float max = IMPORTER->ReadFloat();
		int myJointIndex = IMPORTER->ReadInt();

		XMFLOAT4 xmf4Pos;
		xmf4Pos.x = IMPORTER->ReadFloat();
		xmf4Pos.y = IMPORTER->ReadFloat();
		xmf4Pos.z = IMPORTER->ReadFloat();
		xmf4Pos.w = 1.0f;

		XMFLOAT4 xmf4Scale;
		xmf4Scale.x = IMPORTER->ReadFloat();
		xmf4Scale.y = IMPORTER->ReadFloat();
		xmf4Scale.z = IMPORTER->ReadFloat();
		xmf4Scale.w = 1.0f;

		XMFLOAT4 xmf4Quaternion;
		xmf4Quaternion.x = IMPORTER->ReadFloat();
		xmf4Quaternion.y = IMPORTER->ReadFloat();
		xmf4Quaternion.z = IMPORTER->ReadFloat();
		xmf4Quaternion.w = IMPORTER->ReadFloat();
		pBoundingBox->Begin(XMLoadFloat4(&xmf4Pos), XMLoadFloat4(&xmf4Scale), XMLoadFloat4(&xmf4Quaternion));
		//pBoundingBox->SetPosition(XMLoadFloat3(&xmf3Pos));

		pBoundingBox->SetMin(min);
		pBoundingBox->SetMax(max);
		pBoundingBox->SetMyJointIndex(myJointIndex);
		pAnimationInfo->GetActiveOBB().push_back(pBoundingBox);
	}//obb for end
	float animationSpd = IMPORTER->ReadFloat();
	pAnimationInfo->SetAnimationSpd(animationSpd);

	CAnimationData* pAnimationData = new CAnimationData();
	int jointCnt = pAnimater->GetSkeletonData()->GetJointCnt();
	int frameCnt = IMPORTER->ReadInt();
	pAnimationData->SetAnimationLength(frameCnt);

	for (int j = 0; j < jointCnt; ++j) {
		int curJointFrameCnt = IMPORTER->ReadInt();
		pAnimationData->GetAllKeyFrame().resize(jointCnt);
		if (curJointFrameCnt <= 0) continue;
		for (int k = 0; k < frameCnt; ++k) {
			CKeyFrame keyFrame;
			keyFrame.SetKeyFrameTransformMtx(IMPORTER->ReadXMMatrix());
			pAnimationData->GetAllKeyFrame()[j].push_back(keyFrame);
		}//end frame for
	}//end joint for

	pAnimationInfo->SetAnimationData(pAnimationData);
	pAnimationInfo->Begin(pAnimater);
	return pAnimationInfo;
}

CAnimationInfo::CAnimationInfo(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("animationinfo", pd3dDevice, pd3dDeviceContext){

}

CAnimationInfo::~CAnimationInfo()
{
}
