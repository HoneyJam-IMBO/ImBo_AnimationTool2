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

struct DS_OUTPUT
{
	float4 Position  : SV_POSITION;
	uint2 UV : TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: 다른 내용을 변경/추가합니다.
};

float4 main(DS_OUTPUT input) : SV_TARGET{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
//PS_GBUFFER_OUT main(DS_OUTPUT input) 
//{
//	float4 cColor = float4(1.0,1.0,1.0,1.0);
//
//	float Depth = input.Position.z / input.Position.w;
//
//	//return (PackGBuffer(cColor, normalize(float3(1, 0, 0)), input.positionW, gSpecIntensity, gSpecExp, Depth));
//	return (PackGBuffer(cColor, normalize(float3(0, 1, 0)), input.Position, gSpecIntensity, gSpecExp, Depth));
//
//}