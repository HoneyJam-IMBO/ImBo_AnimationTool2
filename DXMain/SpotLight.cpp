#include "stdafx.h"
#include "SpotLight.h"

bool CSpotLight::Begin(SPOT_LIGHT& light_info) {
	m_lightid = light_id::LIGHT_SPOT;
	m_objectID = object_id::OBJECT_SPOT_LIGHT;

	m_SpotData = light_info;

	//미리 계산 가능한 것은 계산 해 놓는다.
	float fCosInnerCone = cosf(XM_PI * m_SpotData.fInnerAngle / 180.0f);
	float fCosOuterCone = cosf(XM_PI * m_SpotData.fOuterAngle / 180.0f);
	float fSinOuterCone = sinf(XM_PI * m_SpotData.fOuterAngle / 180.0f);

	m_fSpotLightRangeRcp = 1.0f / m_SpotData.SpotLightRange;
	m_fSpotCosOuterCone = fCosOuterCone;
	m_fSpotSinOuterCone = fSinOuterCone;
	m_fSpotCosConeAttRcp = 1.0f / (fCosInnerCone - fCosOuterCone);

	float SpotLightRange = m_SpotData.SpotLightRange;
	m_xmmtxScale = XMMatrixScalingFromVector(XMVECTOR(XMVectorSet(SpotLightRange, SpotLightRange, SpotLightRange, 1.0f)));


	////새로운 객체가 생겨나면 Begin에서 Component들을 set해준다. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//SetComponent(pComponent);

	return CLight::Begin();
}
bool CSpotLight::End() {

	return CLight::End();
}

//instance buffer controll base
void CSpotLight::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {

	//형변환
	SPOT_LIGHT_DS_CB *pDS_InstanceData = (SPOT_LIGHT_DS_CB*)ppMappedResources[0];
	SPOT_LIGHT_PS_CB *pPS_InstanceData = (SPOT_LIGHT_PS_CB*)ppMappedResources[1];
	//transpose 이후 정보 주입
	
	//light projection 이 작업 gpu로 옮겨야함 later
	XMMATRIX xmmtxWorld = GetWorldMtx();
	XMMATRIX xmmtxView = pCamera->GetViewMtx();
	XMMATRIX xmmtxProjection = pCamera->GetProjectionMtx();

	XMMATRIX xmmtxLight = m_xmmtxScale * xmmtxWorld * xmmtxView * xmmtxProjection;

	pDS_InstanceData[nInstance].LightProjection = XMMatrixTranspose(xmmtxLight);
	pDS_InstanceData[nInstance].SpotCosOuterCone = m_fSpotCosOuterCone;
	pDS_InstanceData[nInstance].SpotSinOuterCone = m_fSpotSinOuterCone;

	//light projection
	XMStoreFloat3(&pPS_InstanceData[nInstance].SpotLightPos, GetPosition());
	pPS_InstanceData[nInstance].SpotLightRangeRcp = m_fSpotLightRangeRcp;
	pPS_InstanceData[nInstance].SpotLightColor = m_SpotData.SpotLightColor;
	pPS_InstanceData[nInstance].SpotCosOuterCone  = m_fSpotCosOuterCone;
	pPS_InstanceData[nInstance].SpotCosConeAttRcp = m_fSpotCosConeAttRcp;
	XMStoreFloat3(&pPS_InstanceData[nInstance].SpotLightDir, -GetLook());

}

void CSpotLight::SetLength(float len){
	m_SpotData.SpotLightRange = len;

	float SpotLightRange = m_SpotData.SpotLightRange;
	m_xmmtxScale = XMMatrixScalingFromVector(XMVECTOR(XMVectorSet(SpotLightRange, SpotLightRange, SpotLightRange, 1.0f)));

}

void CSpotLight::SetRange(float outer, float inner){
	
	m_SpotData.fInnerAngle = MIN(outer, inner);
	m_SpotData.fOuterAngle = outer;

	//미리 계산 가능한 것은 계산 해 놓는다.
	float fCosInnerCone = cosf(XM_PI * m_SpotData.fInnerAngle / 180.0f);
	float fCosOuterCone = cosf(XM_PI * m_SpotData.fOuterAngle / 180.0f);
	float fSinOuterCone = sinf(XM_PI * m_SpotData.fOuterAngle / 180.0f);

	m_fSpotLightRangeRcp = 1.0f / m_SpotData.SpotLightRange;
	m_fSpotCosOuterCone = fCosOuterCone;
	m_fSpotSinOuterCone = fSinOuterCone;
	m_fSpotCosConeAttRcp = 1.0f / (fCosInnerCone - fCosOuterCone);

}

void CSpotLight::SetColor(float r, float g, float b){
	m_SpotData.SpotLightColor = XMFLOAT3(r,g,b);
}

CSpotLight::CSpotLight() {
	
}
CSpotLight::~CSpotLight() {

}