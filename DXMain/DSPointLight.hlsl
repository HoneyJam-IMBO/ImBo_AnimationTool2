#define MAX_POINT_LIGHT 800

struct PointLight {
	float4x4 LightProjection;
};

cbuffer cbPointLightDomain : register(b0) {
	PointLight gPointLight[MAX_POINT_LIGHT];
}


struct DS_OUTPUT
{
	float4 Position	: SV_POSITION;
	uint2 UV		: TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: 다른 내용을 변경/추가합니다.
};

// 제어점을 출력합니다.
struct HS_CONTROL_POINT_OUTPUT
{
	float4 HemiDir : POSITION; //position + instanceID
	uint instanceID : TEXCOORD0;
};

// 패치 상수 데이터를 출력합니다.
struct HS_CONSTANT_DATA_OUTPUT
{
	float Edges[4]			: SV_TessFactor; // 예를 들어 쿼드 도메인에 대해 [4]입니다.
	float Inside[2]			: SV_InsideTessFactor; // 예를 들어 쿼드 도메인에 대해 Inside[2]입니다.
	// TODO: 다른 내용을 변경/추가합니다.
};

//테셀레이션이 생성한 각 정점에 대한 변환을 처리해야함 각 정점!!
[domain("quad")]
DS_OUTPUT main( HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_CONTROL_POINT_OUTPUT, 1> quad){


	//클립 좌표 구함
	float2 posClipSpace = UV.xy * 2.0 - 1.0;

	//중심에서 가장 먼 지점의 절댓값 거리 계산
	float2 posClipSpaceAbs = abs(posClipSpace.xy);
	float maxLen = max(posClipSpaceAbs.x, posClipSpaceAbs.y);

	//클립 공간에서의 최종 위치 생성
	float3 normDir = normalize(float3(posClipSpace.xy, maxLen - 1.0) * quad[0].HemiDir.xyz);
	float4 posLS = float4(normDir.xyz, 1.0);

	//프로젝션 고안에 대해 모두 변환한 후
	//UV 좌표 생성
	DS_OUTPUT Output;
	Output.Position = mul(posLS, gPointLight[quad[0].HemiDir.w].LightProjection);
	//Output.Position = posLS;

	//클립 공간 위치 저장
	Output.UV = Output.Position.xy / Output.Position.w;

	//instanceID get!!
	Output.instanceID = quad[0].HemiDir.w;
	return Output;

}
