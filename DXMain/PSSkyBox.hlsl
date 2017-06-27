#include "PackGbuffer.hlsli"

TextureCube gtxtSkyBox : register(t13);
SamplerState gssSkyBox : register(s0);

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


struct VS_SKYBOX_CUBEMAP_OUTPUT
{
	float3	positionL : POSITION;
	float3 positionW : TEXCOORD0;
	float4	position : SV_POSITION;
};

float4 main(VS_SKYBOX_CUBEMAP_OUTPUT input) : SV_Target
{

	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;

float4 cColor = gtxtSkyBox.Sample(gssSkyBox, input.positionL);
return cColor;
//sky box 조명 연산을 위한 render..
//float Depth = input.position.z / input.position.w;
//
//return (PackGBuffer(cColor, normalize(float3(1, 0, 0)), input.positionW, gSpecIntensity, gSpecExp, Depth));

}
