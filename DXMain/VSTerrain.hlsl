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

struct VS_INPUT {
	float3 position : POSITION;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4x4 mtxWorld : INSTANCEPOS;
};
struct VS_OUTPUT {
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD;
	float2 detailTexCoord : TEXCOORD1;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGET;
};

float2 CalcUV(float3 positionW) {
	int indexX = positionW.x * gOneSpaceSizeRcp;
	int indexZ = positionW.z * gOneSpaceSizeRcp;

	indexZ = gOneSideSpaceNum - indexZ;

	float2 uv = float2(clamp(indexX*gOneSideSpaceNumRcp,0,1), clamp(indexZ*gOneSideSpaceNumRcp,0,1));
	return uv;
}

static const float2 arrUV[4] = {
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};

VS_OUTPUT main( VS_INPUT input, uint vertexID : SV_VertexID )
{
	//입력된 모든 controll point에 대해서 이게 실행
	//4개의 controll point는 Terrain의 제일 끝점을 가리킨다 
	VS_OUTPUT output = (VS_OUTPUT)0;
//해당 점을 투영 좌표로 바꾸고 
//그 후 에 TS를 진행하도록 하자
	output.positionW = mul(float4(input.position, 1.0f), input.mtxWorld).xyz;
	output.texCoord = CalcUV(output.positionW);
	output.detailTexCoord = arrUV[vertexID];
	output.tangentW = mul(input.tangent, (float3x3)input.mtxWorld);
	output.bitangentW = mul(input.bitangent, (float3x3)input.mtxWorld);

	return output;
}