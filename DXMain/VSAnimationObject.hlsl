
cbuffer cbWorldMtx : register(b1) {
	float4x4 mtxWorld : packoffset(c0);
};

cbuffer cbSkinned : register(b10){
	// 한 캐릭터당 최대 뼈대 개수는 96
	float4x4 gmtxBoneTransforms[256] : packoffset(c0);
};

cbuffer ViewProjectionConstantBuffer : register(b0)
{
	matrix gmtxViewProjection;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 boneWeight : BONE_WEIGHT;
	float4 boneIndex : BONE_INDEX;
};

// 픽셀 셰이더를 통과한 픽셀당 색 데이터입니다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangentW : TANGENT;
	float3 binormalW : BINORMAL;
};


VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
	// 배열을 초기화한다. 그렇지 않으면 SV_POSITION에 관련한 경고 메시지가 나온다.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.boneWeight.x;
	weights[1] = input.boneWeight.y;
	weights[2] = input.boneWeight.z;
	weights[3] = saturate(1.0f - (weights[0] + weights[1] + weights[2]));//input.weights.w;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	float3 tangentL = float3(0.0f, 0.0f, 0.0f);
	float3 binormalL = float3(0.0f, 0.0f, 0.0f);
	// 정점 블랜딩 수행
	for (int i = 0; i < 4; ++i){
		posL += mul(float4(input.position, 1.0f), gmtxBoneTransforms[input.boneIndex[i]]).xyz * weights[i];
		normalL += mul(input.normal, (float3x3)gmtxBoneTransforms[input.boneIndex[i]]) * weights[i];
		tangentL += mul(input.tangent, (float3x3)gmtxBoneTransforms[input.boneIndex[i]]) * weights[i];
		binormalL += mul(input.binormal, (float3x3)gmtxBoneTransforms[input.boneIndex[i]]) * weights[i];
	}
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.positionW = mul(float4(posL, 1.0f), mtxWorld).xyz;
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
	output.normalW = mul(normalL, (float3x3)mtxWorld);
	output.tangentW = mul(tangentL, (float3x3)mtxWorld);
	output.binormalW = mul(binormalL, (float3x3)mtxWorld);
	output.uv = input.uv;

	return output;
}
