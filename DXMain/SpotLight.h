#pragma once

#include "Light.h"
#include "RotateComponent.h"

struct SPOT_LIGHT_DS_CB{
	XMMATRIX LightProjection;
	float SpotCosOuterCone;//밖 범위 설정<OuterAngle>
	float SpotSinOuterCone;//<OuterAngle>
	XMFLOAT2 tempData;//float4를 맞추기 위한 데이터
};

struct SPOT_LIGHT_PS_CB {
	XMFLOAT3 SpotLightPos;//설정
	float SpotLightRangeRcp;//범위 설정

	XMFLOAT3 SpotLightDir;
	float SpotCosOuterCone;//밖 범위 설정<OuterAngle>

	XMFLOAT3 SpotLightColor;//색 설정
	float SpotCosConeAttRcp;//안쪽 범위 설정<InnerAngle>
};

struct SPOT_LIGHT {
	float SpotLightRange;
	XMFLOAT3 SpotLightColor;
	float fOuterAngle;
	float fInnerAngle;
};
class CSpotLight : public CLight {
public:
	bool Begin(SPOT_LIGHT& light_info);
	virtual bool End();

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

	//light info setter
	virtual void SetLength(float len);
	virtual void SetRange(float outer, float inner = 0.0f);
	virtual void SetColor(float r, float g, float b);//color
private:
	float m_fSpotLightRangeRcp{ 0.0 };
	float m_fSpotCosOuterCone{ 0.0 };
	float m_fSpotSinOuterCone{ 0.0 };
	float m_fSpotCosConeAttRcp{ 0.0 };

	//data
	SPOT_LIGHT m_SpotData{};
	//data

	XMMATRIX m_xmmtxScale;
public:
	CSpotLight();
	virtual ~CSpotLight();
};