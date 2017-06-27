cbuffer ViewProjectionConstantBuffer : register(b0)
{
	matrix gmtxViewProjection;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR0;
	float4x4 mtxWorld : INSTANCEPOS;
};

// 픽셀 셰이더를 통과한 픽셀당 색 데이터입니다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};


VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(mul(float4(input.position, 1.0f), input.mtxWorld), gmtxViewProjection);
	//output.position = mul(float4(input.position, 1.0f), gmtxViewProjection);
	output.color = input.color;
	//output.color = float4(1.0f,1.0f,0.0f, 1.0f);

	return output;
}