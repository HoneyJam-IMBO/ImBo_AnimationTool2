
texture2D gtxtViewer : register(t0);
sampler gssViewer : register(s0);

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{
	float4 cColor = gtxtViewer.Sample(gssViewer, input.texCoord);
	return cColor;
}