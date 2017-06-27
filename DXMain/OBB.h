#pragma once

#include "GameObject.h"


struct VS_VB_OBB_INSTANCE {
	XMFLOAT3 m_xmf3Pos;
	XMFLOAT3 m_xmf3Extend;
	XMFLOAT4 m_xmf4Quaternion;
};

class OBB : public CGameObject {
public:
	//debug buffer controll
	virtual void SetDebugBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);
	//buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

	void SetBoundingBoxInfo(CGameObject* pObject);

	virtual bool IsVisible(shared_ptr<CCamera> pCamera) { return true; };//계층구조의 녀석들은 다시 만들어줄 필요가 있음
private:

public:
	OBB();
	~OBB();
};
