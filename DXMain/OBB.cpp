#include "stdafx.h"
#include "OBB.h"

void OBB::SetDebugBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera) {
	//형변환
	VS_VB_OBB_INSTANCE* pnInstances = (VS_VB_OBB_INSTANCE*)ppMappedResources[0];

	////정보 주입
	//pnInstances[nInstance].m_xmf3Pos = m_BoundingBox.Center;
	//pnInstances[nInstance].m_xmf3Extend = m_BoundingBox.Extents;
	//pnInstances[nInstance].m_xmf4Quaternion = m_xmf4RotationQuaternion;

	//CGameObject::Begin();
}

void OBB::SetBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera) {
	//형변환
	VS_VB_AABB_INSTANCE* pnInstances = (VS_VB_AABB_INSTANCE*)ppMappedResources[0];

	//XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f));
	//XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionIdentity());
	////정보 주입
	//pnInstances[nInstance].m_xmf3Pos = m_BoundingBox.Center;
	//pnInstances[nInstance].m_xmf3Extend = m_BoundingBox.Extents;
	//pnInstances[nInstance].m_xmf4Quaternion = m_xmf4RotationQuaternion;
}

void OBB::SetBoundingBoxInfo(CGameObject * pObject) {
	//m_BoundingBox = pObject->GetBoundingBox();
	//XMStoreFloat4(&m_xmf4RotationQuaternion, pObject->GetRotationQuaternion());
}

OBB::OBB() :CGameObject("obb") {
	
}

OBB::~OBB()
{
}
