

static const float2 arrBasePos[4] = {
	float2(-1.0, -1.0),
	float2(-1.0, 1.0),
	float2(1.0, -1.0),
	float2(1.0, 1.0)
};
static const float2 arrUV[4] = {
	float2(0, 1),
float2(0, 0),
float2(1, 1),
float2(1, 0)
};
//
struct VS_DIRECTIONAL_OUTPUT {
	float4 position : SV_Position;
	float2 uv : TEXCOORD0;
};

//¥Ÿ¡ﬂ ∑ª¥ı≈∏∞Ÿ
VS_DIRECTIONAL_OUTPUT  main(uint vertexID : SV_VertexID)
{
	VS_DIRECTIONAL_OUTPUT  output = (VS_DIRECTIONAL_OUTPUT)0;
	output.position = float4(arrBasePos[vertexID], 0.0f, 1.0f);
	output.uv = arrUV[vertexID];

	return(output);
}
