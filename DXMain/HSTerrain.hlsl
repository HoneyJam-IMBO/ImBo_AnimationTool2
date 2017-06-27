// 제어점을 입력합니다.
struct VS_OUTPUT {
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD;
	float2 detailTexCoord : TEXCOORD1;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGET;
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

// 패치 상수 데이터를 출력합니다.
struct TERRAIN_HS_CONSTANT
{
	float fTessEdges[4]			: SV_TessFactor; // 예를 들어 쿼드 도메인에 대해 [4]입니다.
	float fTessInsides[2]		: SV_InsideTessFactor; // 예를 들어 쿼드 도메인에 대해 Inside[2]입니다.
	// TODO: 다른 내용을 변경/추가합니다.
};

// 패치 상수 함수
TERRAIN_HS_CONSTANT ConstantHS(InputPatch<VS_OUTPUT, 4> input, uint nPatchID : SV_PrimitiveID)
{
	TERRAIN_HS_CONSTANT output;
	float fOutTess = 128;
	float fInsideTess = 64;

	output.fTessEdges[0] = fOutTess;
	output.fTessEdges[1] = fOutTess;
	output.fTessEdges[2] = fOutTess;
	output.fTessEdges[3] = fOutTess;
	output.fTessInsides[0] = fInsideTess;
	output.fTessInsides[1] = fInsideTess;

	return output;
}


[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
TERRAIN_HS_OUTPUT main(InputPatch<VS_OUTPUT, 4> input, uint i : SV_OutputControlPointID)
{
	TERRAIN_HS_OUTPUT output;
	output.positionW = input[i].positionW;
	output.texCoord = input[i].texCoord;
	output.detailTexCoord = input[i].detailTexCoord;
	output.tangentW = input[i].tangentW;
	output.bitangentW = input[i].bitangentW;
	return output;
}
