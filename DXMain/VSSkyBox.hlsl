cbuffer ViewProjectionConstantBuffer : register(b0)
{
	matrix gmtxViewProjection;
};

struct VS_SKYBOX_CUBEMAP_INPUT
{
	float3	position : POSITION;
	float2 texCoord : TEXCOORD0;
	float4x4 mtxWorld : INSTANCEPOS;
};


struct VS_SKYBOX_CUBEMAP_OUTPUT
{
	float3	positionL : POSITION;
	float3 positionW : TEXCOORD0;
	float4	position : SV_POSITION;
};


VS_SKYBOX_CUBEMAP_OUTPUT main(VS_SKYBOX_CUBEMAP_INPUT input)
{
	
	VS_SKYBOX_CUBEMAP_OUTPUT output = (VS_SKYBOX_CUBEMAP_OUTPUT)0;
	output.positionW = mul(float4(input.position, 1.0f), input.mtxWorld).xyz;
	output.position = mul(float4(output.positionW,1.0f), gmtxViewProjection);
	output.positionL = input.position;

	return(output);
}