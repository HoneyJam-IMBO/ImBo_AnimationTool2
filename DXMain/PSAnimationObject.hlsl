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


//texture
Texture2D    gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);

Texture2D    gtxtSpec : register(t1);
SamplerState gssSpec : register(s1);

Texture2D	gtxtCP : register(t2);
Texture2D	gtxtNormal : register(t3);

//texture

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangentW : TANGENT;
	float3 binormalW : BINORMAL;
};

// (보간된) 색 데이터에 대한 통과 함수입니다.
PS_GBUFFER_OUT main(PixelShaderInput input)
{
	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;

	float4 cCPColor = gtxtCP.Sample(gssDefault, input.uv);
	clip(cCPColor.g < 0.05f ? -1 : 1);

	float4 cColor = gtxtDefault.Sample(gssDefault, input.uv) * gMaterialColor;
	float4 cSpecColor = gtxtSpec.Sample(gssSpec, input.uv) * gMaterialColor;
	//cColor *= cSpecColor;
	

	float3 T = normalize(input.tangentW);
	float3 B = normalize(input.binormalW);
	float3 N = normalize(input.normalW);

	float3x3 TBN = float3x3(T, B, N);
	float3 normal = gtxtNormal.Sample(gssDefault, input.uv).xyz;
	normal = 2.0f * normal - 1.0f;
	float3 normalW = mul(normal, TBN);

	float Depth = input.position.z / input.position.w;

	return (PackGBuffer(cColor.xyz, normalize(normalW), input.positionW, cSpecColor, gSpecExp, Depth));
}
//float4 main(PixelShaderInput input) : SV_TARGET
//{
//
//return gMaterialColor;
//
//}
