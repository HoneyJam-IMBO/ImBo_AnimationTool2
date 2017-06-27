#include "PackGbuffer.hlsli"

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}

struct GS_OUT {
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
};

float4 main(GS_OUT input) : SV_TARGET{
	return float4(1.0f, 0.5f, 0.0f, 1.0f);
}

//PS_GBUFFER_OUT main(GS_OUT input) : SV_TARGET
//{
//	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;
//
////float4 cColor = gtxtSkyBox.Sample(gssSkyBox, input.positionL);
//float4 cColor = float4(1.0, 1.0, 1.0, 1.0);
//
//float Depth = input.position.z / input.position.w;
//
//return (PackGBuffer(cColor, normalize(float3(0, 1, 0)), input.positionW, gSpecIntensity, gSpecExp, Depth));
//}
