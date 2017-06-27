#pragma once
#include "Object.h"
#include "Space.h"
#include "Camera.h"
//#include "RenderContainerSeller.h"


class CSpaceContainer : public CObject {
public:
	void Begin(int size, int lv);
	bool End();

	//space controller
	void Animate(float fTimeElapsed);
	void PrepareRender(shared_ptr<CCamera> pCamera);
	//animate하다가 해당 공간을 벗어난 객체 임시 저장소
	void AddBlockObjectList(CGameObject* pObject);
	void AddObject(CGameObject* pObject);
	void RevomeObject(CGameObject* pObject);
	void ClearBlockObjectList();
	int SearchSpace(XMVECTOR xmvPos);
	void AddSpace(UINT index, CSpace* pSpace);
	//space controller

	//space
	list<CGameObject*>& GetBlockObjectList() { return m_lpBlockObject; }
	CSpace* GetStartSpace() { return m_pStartSpace; }
	UINT GetSpaceNum() { return m_nSpace; }
	UINT GetSize() { return m_size; }
	UINT GetLevel() { return m_level; }
	UINT GetOneSideSpaceNum() { return m_oneSideSpaceNum; }
	UINT GetOneSpaceSize() { return m_oneSpaceSize; }
	//자신이 있는 공간을 찾는 함수 해당 공간의 번호를 리턴한다.
	

	//해당 공간의 인덱스 xyz를 spacenum으로 바꾸는 함수
	//int GetSpaceIndexNum(Index& index);
	//해당 공간의 인덱스가 유효한지 검사하는 함수
	//bool CheckSpaceNum(Index& index);

	//모든 lay 충돌 검사된 객체 중 가장 가장 가까운 객체
	CGameObject* PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distanse);
private:
	
	//space들을 관리한다.
	CSpace** m_ppSpace{ nullptr };
	//쿼드 트리의 루트노드에 해당하는 startSpace 관리
	CSpace* m_pStartSpace{ nullptr };
	//block object list
	list<CGameObject*> m_lpBlockObject;

	//전체 공간 크기
	int m_size{ 0 };
	//공간의 레벨 
	int m_level{ 0 };
	//한 사이드에 있는 공간의 개수
	int m_oneSideSpaceNum{ 0 };
	//공간 하나의 크기
	int m_oneSpaceSize{ 0 };
	//공간의 개수
	int m_nSpace{ 0 };

public:
	CSpaceContainer();
	~CSpaceContainer();
};