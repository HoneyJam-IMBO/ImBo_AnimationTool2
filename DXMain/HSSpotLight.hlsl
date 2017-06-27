

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	uint instanceID : TEXCOORD0;
};


// 제어점을 출력합니다.
struct HS_CONTROL_POINT_OUTPUT
{
	float4 CapsuleDir : POSITION; //position + instanceID
	uint instanceID : TEXCOORD0;
};
//capsule

// 패치 상수 데이터를 출력합니다.
struct HS_CONSTANT_DATA_OUTPUT
{
	float Edges[4] : SV_TessFactor; // 예를 들어 쿼드 도메인에 대해 [4]입니다.
	float Inside[2] : SV_InsideTessFactor; // 예를 들어 쿼드 도메인에 대해 Inside[2]입니다.
										   // TODO: 다른 내용을 변경/추가합니다.
};

// 패치 상수 함수
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants()
{
	HS_CONSTANT_DATA_OUTPUT Output;

	float tessFactor = 18.0f;

	// 여기에 출력을 계산할 코드를 삽입합니다.
	//얼마나 나눌 것인가.
	Output.Edges[0] =
		Output.Edges[1] =
		Output.Edges[2] =
		Output.Edges[3] = tessFactor;

	Output.Inside[0] = Output.Inside[1] = tessFactor;
	// 예를 들어 대신 동적 공간 분할(tessellation) 인수를 계산할 수 있습니다.

	return Output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main(InputPatch<VS_OUTPUT, 1> input, uint PatchID : SV_PrimitiveID)
{
	HS_CONTROL_POINT_OUTPUT Output;

	uint instanceID = input[0].instanceID;
	// 여기에 출력을 계산할 코드를 삽입합니다.
	Output.CapsuleDir = input[0].position;
	Output.instanceID = instanceID;
	return Output;
}
