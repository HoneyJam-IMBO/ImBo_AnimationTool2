static const float2 arrUV[4] = {
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};
static const float2 arrBasePos[4] = {
	float2(-1.0, -1.0),
	float2(-1.0, 1.0),
	float2(1.0, -1.0),
	float2(1.0, 1.0)
};

struct VS_OUTPUT {
	float2 fLeftTop : LEFTTOP;
	float2 fRightBottom : RIGHTBOTTOM;
};

struct GS_OUTPUT{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

[maxvertexcount(4)]
void main( point VS_OUTPUT input[1], inout TriangleStream< GS_OUTPUT > stream)
{
	GS_OUTPUT output[4];

	float left = input[0].fLeftTop.x;
	float top = input[0].fLeftTop.y;
	float right = input[0].fRightBottom.x;
	float bottom = input[0].fRightBottom.y;
	
	output[0].pos = float4(left, bottom, 0.0, 1.0); 
	output[1].pos = float4(left, top, 0.0, 1.0);
	output[2].pos = float4(right, bottom, 0.0, 1.0); 
	output[3].pos = float4(right, top, 0.0, 1.0);

	for (uint i = 0; i < 4; i++)
	{
		output[i].uv = arrUV[i];
		stream.Append(output[i]);
	}

}