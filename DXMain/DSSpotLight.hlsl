#define MAX_SPOT_LIGHT 800

//spot
#define CylinderPortion 0.2
#define ExpendAmount (1.0 + CylinderPortion)

struct SpotLight {
	float4x4 LightProjection;

	float SpotCosOuterCone;
	float SpotSinOuterCone;
	float2 tempData;//float4를 맞추기 위한 데이터
};

cbuffer cbSpotLightDomain : register(b0) {
	SpotLight  gSpotLight[MAX_SPOT_LIGHT];
}
//spot

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
	
	//원뿔 버텍스를 메시 에지로 강제 변환
	float2 posClipSpaceNoCylAbs = saturate(posClipSpaceAbs * ExpendAmount);
	float maxLenNoCapsule = max(posClipSpaceNoCylAbs.x, posClipSpaceNoCylAbs.y);
	float2 posClipSpaceNoCyl = sign(posClipSpace.xy) * posClipSpaceNoCylAbs;
	
	//반구 위치를 가장자리의 원뿔 버텍스로 변환
	float3 halfSpherePos = normalize(float3(posClipSpaceNoCyl.xy, 1.0 - maxLenNoCapsule));

	//구를 원뿔 밑면 크기로 스케일 조정
	halfSpherePos = normalize(float3(halfSpherePos.xy * gSpotLight[instanceID].SpotSinOuterCone, gSpotLight[instanceID].SpotCosOuterCone));

	//원뿔 버텍스의 오프셋 계산 ( 원뿔 밑면은 0 )
	float cylinderOffsetZ = saturate((maxLen * ExpendAmount - 1.0) / CylinderPortion);

	//원뿔 버텍스를 최종 위치로 오프셋
	float4 posLS = float4(halfSpherePos.xy * (1.0 - cylinderOffsetZ), halfSpherePos.z - cylinderOffsetZ * gSpotLight[instanceID].SpotCosOuterCone, 1.0);


	//투영 공간에 대해 모두 변환한 후
	//UV 좌표 생성
	DS_OUTPUT Output;
	Output.Position = mul(posLS, gSpotLight[instanceID].LightProjection);
	Output.UV = Output.Position.xy / Output.Position.w;
	//Output.UV = Output.UV * float2(0.5, -0.5) + 0.5;
	Output.instanceID = instanceID;

	return Output;
}
