#include "stdafx.h"
#include "BoundingBox.h"

bool CBoundingBox::Begin(XMVECTOR Position, XMVECTOR Extend, XMVECTOR Quaternion) {
	SetPosition(Position);
	SetScale(Extend);
	XMStoreFloat4(&m_xmf4Quaternion, Quaternion);
	return true;
}

void CBoundingBox::SetDebugBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera){
	//형변환
	VS_VB_BOUNDINGBOX_INSTANCE* pnInstances = (VS_VB_BOUNDINGBOX_INSTANCE*)ppMappedResources[0];

	//정보 주입
	pnInstances[nInstance].m_xmf3Pos = m_xmf3Position;
	pnInstances[nInstance].m_xmf3Extend = XMFLOAT3(m_xmf4Scale.x, m_xmf4Scale.y, m_xmf4Scale.z);
	pnInstances[nInstance].m_xmf4Quaternion = m_xmf4Quaternion;

	//CGameObject::Begin();
}

void CBoundingBox::SetBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera){
	//형변환
	VS_VB_BOUNDINGBOX_INSTANCE* pnInstances = (VS_VB_BOUNDINGBOX_INSTANCE*)ppMappedResources[0];

	//XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f));
	//XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionIdentity());
	//정보 주입
	pnInstances[nInstance].m_xmf3Pos = m_xmf3Position;
	pnInstances[nInstance].m_xmf3Extend = XMFLOAT3(m_xmf4Scale.x, m_xmf4Scale.y, m_xmf4Scale.z);
	pnInstances[nInstance].m_xmf4Quaternion = m_xmf4Quaternion;
}

void CBoundingBox::SetBoundingBoxInfo(BoundingBox& aabb){
	m_xmf3Position = aabb.Center;
	m_xmf4Scale = XMFLOAT4(aabb.Extents.x, aabb.Extents.y, aabb.Extents.z, 1.f);
	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity()); 
}
void CBoundingBox::SetBoundingBoxInfo(BoundingOrientedBox& obb) {
	m_xmf3Position = obb.Center;
	m_xmf4Scale = XMFLOAT4(obb.Extents.x, obb.Extents.y, obb.Extents.z, 1.f);
	m_xmf4Quaternion = obb.Orientation;
}

void CBoundingBox::SetBoundingBoxInfo(CBoundingBox & boundingBox){
	SetPosition(boundingBox.GetPosition());
	SetScale(boundingBox.GetScale());
	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}

BoundingBox CBoundingBox::GetAABB(){
	BoundingBox aabb;
	aabb.Center = m_xmf3Position;

	XMStoreFloat3(&aabb.Extents, GetScale());
	return aabb;
}

BoundingOrientedBox CBoundingBox::GetOBB(){
	BoundingOrientedBox obb;
	obb.Center = m_xmf3Position;
	XMStoreFloat3(&obb.Extents, GetScale());
	obb.Orientation = m_xmf4Quaternion;
	return obb;
}

CBoundingBox::CBoundingBox() :CGameObject("boundingbox"){
	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
	XMStoreFloat3(&m_xmf3Position, XMQuaternionIdentity());
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}

CBoundingBox::~CBoundingBox()
{
}
