#include "stdafx.h"
#include "SpaceContainer.h"

void CSpaceContainer::Begin(int size, int lv){
	//크기 설정 
	m_size = size;
	//레벨 설정
	m_level = lv;
	//한 사이드에 있는 공간의 개수
	m_oneSideSpaceNum = static_cast<int>(pow(2, m_level));
	//공간 하나의 크기
	m_oneSpaceSize = size / m_oneSideSpaceNum;
	//공간의 개수 쿼드트리니까 제곱
	m_nSpace = static_cast<int>(pow(m_oneSideSpaceNum, 2));

	//관리되어질 leaf spaces
	m_ppSpace = new CSpace*[m_nSpace];
	
	//공간 할당.
	m_pStartSpace = new CSpace();
	m_pStartSpace->Begin(this, m_size, m_level, XMVectorSet(0.f, 0.f, 0.f, 0.f));

}

bool CSpaceContainer::End(){
	for (auto pObject : m_lpBlockObject) {
		pObject->End();
		delete pObject;
	}

	m_pStartSpace->End();

	return false;
}

void CSpaceContainer::Animate(float fTimeElapsed){
	
	//all space animate
	m_pStartSpace->Animate(fTimeElapsed);

	//Render 그림그리는 space라는 것 초기화
	for (int i = 0; i < m_nSpace; ++i) {
		m_ppSpace[i]->SetbRender(false);
	}

	//animate 이후에 분명히 block object가 등장한다.
	if (false == m_lpBlockObject.empty()) {//block object list가 empty가 아니라면
		for (auto pObject : m_lpBlockObject) {//객체 다시 배치
			AddObject(pObject);
		}
		m_lpBlockObject.clear();
	}
}

void CSpaceContainer::PrepareRender(shared_ptr<CCamera> pCamera){
	m_pStartSpace->PrepareRender(pCamera);
}

void CSpaceContainer::AddBlockObjectList(CGameObject * pObject){
	m_lpBlockObject.emplace_back(pObject);
}

void CSpaceContainer::AddObject(CGameObject * pObject){
	//현재 index 구하기
	int current_index = SearchSpace(pObject->GetPosition());


	if (current_index < 0) {
		current_index = 0;
		pObject->SetPosition(XMVectorSet(0.f, 0.f, 0.f, 0.f));
	}

	//해당 공간에 객체 추가
	m_ppSpace[current_index]->AddObject(pObject);
	//객체에 공간index 등록
	pObject->SetSpaceIndex(current_index);
}

void CSpaceContainer::RevomeObject(CGameObject * pObject){
	for (int i = 0; i < m_nSpace; ++i) {
		m_ppSpace[i]->RemoveObject(pObject);
	}
}

void CSpaceContainer::ClearBlockObjectList() {
	m_lpBlockObject.clear();
}

int CSpaceContainer::SearchSpace(XMVECTOR xmvPos){
	XMFLOAT3 xmf3Pos;
	XMStoreFloat3(&xmf3Pos, xmvPos);
	int x = static_cast<int>(xmf3Pos.x);
	//int y = xmf3Pos.y;
	int z = static_cast<int>(xmf3Pos.z);


	//자신의 위치를 이용해 인덱스를 얻어옴
	int indexX = (int)x / m_oneSpaceSize;
	//int indexY = (int)y / m_oneSpaceSize;
	int indexZ = (int)z / m_oneSpaceSize;



	int index = indexX + (indexZ * m_oneSideSpaceNum);

	//예외 처리 범위 밖
	if (index < 0 || index >= m_nSpace) {
		return -1;
		//return 0;
	}
	if (x < 0 || z < 0 ||
		x > m_size || z > m_size) {
		return -1;
		//return 0;
	}
	//예외 처리 범위 밖


	return index;
}

void CSpaceContainer::AddSpace(UINT index, CSpace * pSpace){
	m_ppSpace[index] = pSpace;
}

CGameObject * CSpaceContainer::PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distanse){
	float fHitDistance = FLT_MAX;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pHitObj = nullptr;
	CGameObject* pNearObj = nullptr;

	//자신의 모든 leaf space에 대해서 검사
	for (int i = 0; i < m_nSpace; ++i) {
		if (m_ppSpace[i]->GetbRender()) {//내가 그림을 그리는 space라면
			pHitObj = m_ppSpace[i]->PickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance);
			if (fNearHitDistance > fHitDistance) {
				fNearHitDistance = fHitDistance;
				pNearObj = pHitObj;
			}
		}
	}
	return pNearObj;
}

CSpaceContainer::CSpaceContainer() : CObject("spacecontainer"){
}

CSpaceContainer::~CSpaceContainer()
{
}

