#include "stdafx.h"
#include "SceneLight.h"

//--------------------dxobject-------------------
bool CSceneLight::Begin() {
	m_ppAmbient = new CAmbient*[MAX_AMBIENT_LIGHT_NUM];

	return DXObject::Begin();
}
bool CSceneLight::End() {
	for (int i = 0; i < MAX_AMBIENT_LIGHT_NUM; ++i) {
		delete m_ppAmbient[i];
	}

	return DXObject::End();
}

void CSceneLight::SetShaderState() {
	

}
void CSceneLight::SetnullptrShaderState() {

}

void CSceneLight::UpdateShaderState() {
	
}
void CSceneLight::SetAmbientLight(XMFLOAT4 AmbientDown, XMFLOAT4 AmbientRange, XMFLOAT4 AmbientUp, XMFLOAT4 AmbientColor) {
	CAmbient* pAmbient = new CAmbient();
	pAmbient->SetAmbientInfo(AmbientDown, AmbientRange, AmbientUp, AmbientColor);
	m_ppAmbient[m_nAmbient++] = pAmbient;
}
CSceneLight::CSceneLight(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject(pd3dDevice, pd3dDeviceContext) {
	 
}
CSceneLight::~CSceneLight() {

}