
texture2D gtxtHeightMap : register(t0);
sampler gssHeightMap : register(s0);

cbuffer ViewProjectionConstantBuffer : register(b0)
{
	matrix gmtxViewProjection;
};
cbuffer GlobalTerrainInfo : register(b10)
{
	float gOneSpaceSizeRcp;//하나의 공간의 크기
	float gOneSideSpaceNumRcp;//1 / 한의 사이드에 있는 공간 수
	float gOneSideSpaceNum;//한의 사이드에 있는 공간 수
	float gHeightScale;
};


// 제어점을 출력합니다.
struct TERRAIN_HS_OUTPUT
{
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD;
	float2 detailTexCoord : TEXCOORD1;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGET;
};
struct DS_OUT {
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD;
	float2 detailTexCoord : TEXCOORD1;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGET;
};

// 패치 상수 데이터를 출력합니다.
struct TERRAIN_HS_CONSTANT
{
	float EdgeTessFactor[4]			: SV_TessFactor; // 예를 들어 쿼드 도메인에 대해 [4]입니다.
	float InsideTessFactor[2]		: SV_InsideTessFactor; // 예를 들어 쿼드 도메인에 대해 Inside[2]입니다.
														   // TODO: 다른 내용을 변경/추가합니다.
};

[domain("quad")]
DS_OUT main(TERRAIN_HS_CONSTANT input, float2 uv : SV_DomainLocation, OutputPatch<TERRAIN_HS_OUTPUT, 4> quad)
{
	DS_OUT output;

	output.positionW = lerp(lerp(quad[0].positionW, quad[1].positionW, uv.x), lerp(quad[2].positionW, quad[3].positionW, uv.x), uv.y);
	output.texCoord = lerp(lerp(quad[0].texCoord, quad[1].texCoord, uv.x), lerp(quad[2].texCoord, quad[3].texCoord, uv.x), uv.y);
	output.detailTexCoord = lerp(lerp(quad[0].detailTexCoord, quad[1].detailTexCoord, uv.x), lerp(quad[2].detailTexCoord, quad[3].detailTexCoord, uv.x), uv.y);
	output.detailTexCoord = output.detailTexCoord * 8;
	output.tangentW = lerp(lerp(quad[0].tangentW, quad[1].tangentW, uv.x), lerp(quad[2].tangentW, quad[3].tangentW, uv.x), uv.y);
	output.bitangentW = lerp(lerp(quad[0].bitangentW, quad[1].bitangentW, uv.x), lerp(quad[2].bitangentW, quad[3].bitangentW, uv.x), uv.y);

	output.positionW.y = gtxtHeightMap.SampleLevel(gssHeightMap, output.texCoord, 0).r * 256 *gHeightScale;//get height 

	//calc _view projection
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);

	return output;
}
