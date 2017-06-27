#define MAX_CAPSULE_LIGHT 800

//capsule
#define CylinderPortion 0.5
#define SpherePortion (1.0 - CylinderPortion)
#define ExpendAmount (1.0 + CylinderPortion)

struct CapsuleLight {
	float4x4 LightProjection;
	
	float CapsuleLightRange;
	float HalfSegmentLen;
	float2 CapsuleTempData;//쓰레기 데이터 float4 마추려고 사용
};

cbuffer cbCapsuleLightDomain : register(b0) {
	CapsuleLight gCapsuleLight[MAX_CAPSULE_LIGHT];
}
//capsule

struct DS_OUTPUT
{
	float4 Position  : SV_POSITION;
	uint2 UV : TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: 다른 내용을 변경/추가합니다.
};

// 제어점을 출력합니다.
struct HS_CONTROL_POINT_OUTPUT
{
	float4 CapsuleDir : POSITION; //position + instanceID
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
DS_OUTPUT main(HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_CONTROL_POINT_OUTPUT, 4> quad) {

	uint instanceID = quad[0].instanceID;
	//클립 좌표 구함
	float2 posClipSpace = UV.xy * float2(2.0, -2.0) + float2(-1.0, 1.0);

	//uv 값에 기반을 두고 버텍스 오프셋 계산
	float2 posClipSpaceAbs = abs(posClipSpace.xy);
	float maxLen = max(posClipSpaceAbs.x, posClipSpaceAbs.y);

	float2 posClipSpaceNoCylAbs = saturate(posClipSpaceAbs * ExpendAmount);
	float maxLenNoCapsule = max(posClipSpaceNoCylAbs.x, posClipSpaceNoCylAbs.y);
	float2 posClipSpaceNoCyl = sign(posClipSpace.xy) * posClipSpaceNoCylAbs;

	float3 halfSpherePos = normalize(float3(posClipSpaceNoCyl.xy, 1.0 - maxLenNoCapsule));

	//클립 공간의 최종위치 생성
	//float cylinderOffsetZ = saturate(maxLen - min(maxLenNoCapsule, SpherePortion)) / CylinderPortion;
	float cylinderOffsetZ = saturate((maxLen * ExpendAmount - 1.0f) / CylinderPortion);
	
	halfSpherePos *= gCapsuleLight[instanceID].CapsuleLightRange;

	float4 posLS = float4(halfSpherePos.xy, halfSpherePos.z 
		+ gCapsuleLight[instanceID].HalfSegmentLen 
		- cylinderOffsetZ * gCapsuleLight[instanceID].HalfSegmentLen, 1.0f);

	//float4 posLS = float4(halfSpherePos.xy, halfSpherePos.z
	//	+ 50
	//	- cylinderOffsetZ * 50, 1.0f);

	//버텍스를 선택한 캡슐 위치로 이동
	posLS *= quad[0].CapsuleDir;

	//투영 공간에 대해 모두 변환한 후
	//UV 좌표 생성
	DS_OUTPUT Output;
	Output.Position = mul(posLS, gCapsuleLight[instanceID].LightProjection);
	Output.UV = Output.Position.xy / Output.Position.w;
	//Output.UV = Output.UV * float2(0.5, -0.5) + 0.5;
	Output.instanceID = instanceID;

	return Output;
}
