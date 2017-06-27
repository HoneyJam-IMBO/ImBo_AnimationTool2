cbuffer ViewProjectionConstantBuffer : register(b0)
{
	matrix gmtxViewProjection;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float4x4 mtxWorld : INSTANCEPOS;
};

// 픽셀 셰이더를 통과한 픽셀당 색 데이터입니다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
};


VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.positionW = mul(float4(input.position, 1.0f), input.mtxWorld).xyz;
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);

	return output;
}
