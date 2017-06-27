#include "stdafx.h"
#include "Light.h"

bool CLight::Begin() { 

	return CGameObject::Begin();
};

//instance buffer controll base
void CLight::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {
	//형변환
	VS_VB_INSTANCE *pnInstances = (VS_VB_INSTANCE *)ppMappedResources[0];
	//transpose 이후 정보 주입
	pnInstances[nInstance].m_xmmtxWorld = XMMatrixTranspose(GetWorldMtx());

}



