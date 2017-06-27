//specpowr 정규화 
static const float2 g_SpecPowerRange = { 0.1, 250.0 };

cbuffer cbgbufferUnpack : register(b0) {
	float4 PerspectiveValues	: packoffset(c0);
	float4x4 ViewInv			: packoffset(c1);
}

SamplerState        gssSamplerState				: register(s0);

Texture2D           gtxtColorSpecInt			: register(t0);
Texture2D           gtxtNormal_Depth			: register(t1);
Texture2D           gtxtPositionW_SpecPow	 	: register(t2);

struct SURFACE_DATA {
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float3 PositionW;
	float SpecInt;
	float SpecPow;
};

//------------------hellper func---------------------
float3 CalcWorldPos(float2 csPos, float linearDepth) {
	float4 position;

	position.xy = csPos.xy * PerspectiveValues.xy * linearDepth;
	position.z = linearDepth;
	position.w = 1.0;

	return mul(position, ViewInv).xyz;
}

float ConvertDepthToLinear(float depth) {
	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);
	return linearDepth;
}
//------------------hellper func---------------------

//------------------unpack gbuffer-------------------
SURFACE_DATA UnpackGBuffer(int2 location) {
	SURFACE_DATA Out = (SURFACE_DATA)0;

	int3 location3 = int3(location, 0);

	//베이스 색상과 스펙큘러 세기 값 추출
	//float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, texCoord);
	float4 baseColorSpecInt = gtxtColorSpecInt.Load(location3);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecInt = baseColorSpecInt.w;

	//노멀 샘플링 후 전체 범위 변환 및 정규화
	float4 normal_depth = gtxtNormal_Depth.Load(location3);
	Out.Normal = normal_depth.xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	//깊이 값 추출 및 선형 깊이 값으로 변환
	float depth = normal_depth.w;
	//Out.LinearDepth = ConvertDepthToLinear(depth);
	Out.LinearDepth = depth;

	//원래 범위 값에 대해 스펙큘러 파워 스케일 조정
	float4 PositionW_SpecPowerNorm = gtxtPositionW_SpecPow.Load(location3);
	Out.PositionW = PositionW_SpecPowerNorm.xyz;
	Out.SpecPow = g_SpecPowerRange.x + PositionW_SpecPowerNorm.w * g_SpecPowerRange.y;

	return Out;
}
/*SURFACE_DATA UnpackGBuffer(float2 texCoord) {
	SURFACE_DATA Out = (SURFACE_DATA)0;

	//베이스 색상과 스펙큘러 세기 값 추출
	float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, texCoord);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecInt = baseColorSpecInt.w;

	//노멀 샘플링 후 전체 범위 변환 및 정규화
	float4 normal_depth = gtxtNormal_Depth.Sample(gssSamplerState, texCoord);
	Out.Normal = normal_depth.xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	//깊이 값 추출 및 선형 깊이 값으로 변환
	float depth = normal_depth.w;
	Out.LinearDepth = ConvertDepthToLinear(depth);

	//원래 범위 값에 대해 스펙큘러 파워 스케일 조정
	float SpecPowerNorm = gtxtSpecPow.Sample(gssSamplerState, texCoord);
	Out.SpecPow = SpecPowerNorm.x + SpecPowerNorm * g_SpecPowerRange.y;

	return Out;
}*/
//------------------unpack gbuffer-------------------
