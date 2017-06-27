#include "stdafx.h"
#include "DirectionalLight.h"

bool CDirectionalLight::Begin(DIRECTIONAL_AMBIENT_LIGHT& light_info) {
	m_lightid = light_id::LIGHT_DIRECTIONAL;
	m_objectID = object_id::OBJECT_DIRECTIONAL_LIGHT;

	//SetLook(XMLoadFloat4(&m_Directional_AmbientData.m_DirToLight));

	m_Directional_AmbientData = light_info;

	return CLight::Begin();
}
bool CDirectionalLight::End() {

	return CLight::End();
}
//instance buffer controll base
void CDirectionalLight::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {
	
	//형변환
	DIRECTIONAL_AMBIENT_LIGHT *pnInstances = (DIRECTIONAL_AMBIENT_LIGHT *)ppMappedResources[0];
	//transpose 이후 정보 주입

	//directional
	XMVECTOR vDir = GetLook();
	XMStoreFloat4(&pnInstances[nInstance].m_DirToLight, -vDir);
	pnInstances[nInstance].m_DirLightColor = m_Directional_AmbientData.m_DirLightColor;
	//pnInstances[nInstance].m_DirToLight = m_Directional_AmbientData.m_DirToLight;
	pnInstances[nInstance].m_DirLightPower = m_Directional_AmbientData.m_DirLightPower;

	//ambient
	pnInstances[nInstance].m_AmbientColor = m_Directional_AmbientData.m_AmbientColor;
	pnInstances[nInstance].m_AmbientDown = m_Directional_AmbientData.m_AmbientDown;
	pnInstances[nInstance].m_AmbientRange = m_Directional_AmbientData.m_AmbientRange;
	pnInstances[nInstance].m_AmbientUp = m_Directional_AmbientData.m_AmbientUp;
}

void CDirectionalLight::SetLength(float len){
	m_Directional_AmbientData.m_AmbientUp = XMFLOAT4(len, len, len, 0.f);
	m_Directional_AmbientData.m_AmbientDown = XMFLOAT4(len, len, len, 0.f);
}

void CDirectionalLight::SetRange(float outer, float inner){
	m_Directional_AmbientData.m_AmbientUp = XMFLOAT4(outer, outer, outer, 0.f);
	m_Directional_AmbientData.m_AmbientDown = XMFLOAT4(inner, inner, inner, 0.f);
}

void CDirectionalLight::SetColor(float r, float g, float b){
	m_Directional_AmbientData.m_DirLightColor = XMFLOAT4(r, g, b, 0.f);
}

bool CDirectionalLight::IsVisible(shared_ptr<CCamera> pCamera){
	//m_BoundingBox = m_OriBoundingBox;
	//m_BoundingBox.Transform(m_BoundingBox, GetWorldMtx());

	return true;
}

CDirectionalLight::CDirectionalLight() {
	
}
CDirectionalLight::~CDirectionalLight() {

}