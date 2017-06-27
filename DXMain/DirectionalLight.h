#pragma once
#include "Light.h"

/*
float3 AmbientDown	: packoffset( c0 );
float3 AmbientRange	: packoffset( c1 );
float3 DirToLight		: packoffset( c2 );
float3 DirLightColor	: packoffset( c3 );
결국 사용하는건 이 4가지뿐. 
*/
struct DIRECTIONAL_AMBIENT_LIGHT {
	XMFLOAT4 m_DirToLight;
	XMFLOAT4 m_DirLightColor;
	XMFLOAT4 m_DirLightPower;

	XMFLOAT4 m_AmbientDown;
	XMFLOAT4 m_AmbientRange;
	XMFLOAT4 m_AmbientUp;
	XMFLOAT4 m_AmbientColor;

};

class CDirectionalLight : public CLight {
public:
	bool Begin(DIRECTIONAL_AMBIENT_LIGHT& light_info);
	virtual bool End();

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

	//light info setter
	virtual void SetLength(float len);
	virtual void SetRange(float outer, float inner = 0.0f);
	virtual void SetColor(float r, float g, float b);//color

	virtual bool IsVisible(shared_ptr<CCamera> pCamera);
private:
	//data
	DIRECTIONAL_AMBIENT_LIGHT m_Directional_AmbientData;
	//data

public:
	CDirectionalLight();
	virtual ~CDirectionalLight();
};