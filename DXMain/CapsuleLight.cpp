#include "stdafx.h"
#include "CapsuleLight.h"

bool CCapsuleLight::Begin(CAPSULE_LIGHT& light_info) {
	m_lightid = light_id::LIGHT_CAPSULE;
	m_objectID = object_id::OBJECT_CAPSULE_LIGHT;

	m_CapsuleData = light_info;

	m_fHalfSegmentLen = m_CapsuleData.CapsuleLightLen*0.5f;
	m_fCapsuleLightRangeRcp = 1 / m_CapsuleData.CapsuleLightRange;

	////새로운 객체가 생겨나면 Begin에서 Component들을 set해준다. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//SetComponent(pComponent);


	return CLight::Begin();
}
bool CCapsuleLight::End() {

	return CLight::End();
}

//instance buffer controll base
void CCapsuleLight::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {

	//형변환
	CAPSULE_LIGHT_DS_CB  *pDS_InstanceData = (CAPSULE_LIGHT_DS_CB*)ppMappedResources[0];
	CAPSULE_LIGHT_PS_CB  *pPS_InstanceData = (CAPSULE_LIGHT_PS_CB*)ppMappedResources[1];


	//light projection
	XMMATRIX xmmtxWorld = GetWorldMtx();
	XMMATRIX xmmtxView = pCamera->GetViewMtx();
	XMMATRIX xmmtxProjection = pCamera->GetProjectionMtx();

	XMMATRIX xmmtxLight = xmmtxWorld * xmmtxView * xmmtxProjection;

	pDS_InstanceData[nInstance].LightProjection = XMMatrixTranspose(xmmtxLight);
	pDS_InstanceData[nInstance].HalfSegmentLen = m_fHalfSegmentLen;
	pDS_InstanceData[nInstance].CapsuleLightRange = m_CapsuleData.CapsuleLightRange;
	//light projection

	//lightpos = capsule start point = pos - vDir * fLen*0.5
	XMVECTOR vDir = GetLook();
	XMVECTOR vPos = GetPosition();
	vPos = vPos - vDir*m_fHalfSegmentLen;
	XMStoreFloat3(&pPS_InstanceData[nInstance].CapsuleLightPos, vPos);
	pPS_InstanceData[nInstance].CapsuleLightLen = m_CapsuleData.CapsuleLightLen;
	pPS_InstanceData[nInstance].CapsuleLightColor = m_CapsuleData.CapsuleLightColor;
	pPS_InstanceData[nInstance].CapsuleLightRangeRcp = m_fCapsuleLightRangeRcp;
//	pPS_InstanceData[nInstance].CapsuleLightIntensity = m_CapsuleData.CapsuleLightIntensity;
	//Dir = Look
	XMStoreFloat3(&pPS_InstanceData[nInstance].CapsuleLightDir, vDir);

}

void CCapsuleLight::SetLength(float len){
	m_CapsuleData.CapsuleLightLen = len;
	m_fHalfSegmentLen = m_CapsuleData.CapsuleLightLen*0.5f;
}

void CCapsuleLight::SetRange(float outer, float inner){
	m_CapsuleData.CapsuleLightRange = MAX(outer, inner);
	m_fCapsuleLightRangeRcp = 1 / m_CapsuleData.CapsuleLightRange;
}

void CCapsuleLight::SetColor(float r, float g, float b){
	m_CapsuleData.CapsuleLightColor = XMFLOAT3(r, g, b);
}


CCapsuleLight::CCapsuleLight() {
	
}
CCapsuleLight ::~CCapsuleLight() {

}