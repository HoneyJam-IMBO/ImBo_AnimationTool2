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

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
};

// (보간된) 색 데이터에 대한 통과 함수입니다.
PS_GBUFFER_OUT main(PixelShaderInput input)
{
	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;

	float4 cColor = gMaterialColor;
	
	float Depth = input.position.z / input.position.w;

	return (PackGBuffer(cColor.xyz, float3(0.f, 1.f, 0.f), input.positionW, gSpecIntensity, gSpecExp, Depth));

}
//float4 main(PixelShaderInput input) : SV_TARGET
//{
//
//return gMaterialColor;
//
//}
