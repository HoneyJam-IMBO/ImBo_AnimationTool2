

SamplerState        gssSamplerState		: register(s0);

Texture2D           gtxtColorSpecInt	: register(t0);
//Texture2D           gtxtNormal_Depth	: register(t1);
//Texture2D           gtxtSpecPow	 		: register(t2);

//static const float2 g_SpecPowerRange = { 0.1, 250.0 };
//
//cbuffer cbgbufferUnpack : register(b0) {
//	float4 PerspectiveValues	: packoffset(c0);
//	float4x4 ViewInv			: packoffset(c1);
//}

//float ConvertDepthToLinear(float depth) {
//	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);
//	return linearDepth;
//}


struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};


struct SURFACE_DATA {
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float SpecInt;
	float SpecPow;
};


//
//SURFACE_DATA UnpackGBuffer(float2 texCoord) {
//	SURFACE_DATA Out = (SURFACE_DATA)0;
//
//	//베이스 색상과 스펙큘러 세기 값 추출
//	float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, texCoord);
//	Out.Color = baseColorSpecInt.xyz;
//	Out.SpecInt = baseColorSpecInt.w;
//
//	//노멀 샘플링 후 전체 범위 변환 및 정규화
//	float4 normal_depth = gtxtNormal_Depth.Sample(gssSamplerState, texCoord);
//	Out.Normal = normal_depth.xyz;
//	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
//
//	//깊이 값 추출 및 선형 깊이 값으로 변환
//	float depth = normal_depth.w;
//	Out.LinearDepth = ConvertDepthToLinear(depth);
//
//	//원래 범위 값에 대해 스펙큘러 파워 스케일 조정
//	float SpecPowerNorm = gtxtSpecPow.Sample(gssSamplerState, texCoord);
//	Out.SpecPow = SpecPowerNorm.x + SpecPowerNorm * g_SpecPowerRange.y;
//
//	return Out;
//}
/*
Texture2D           gtxtColorSpecInt	: register(t0);
Texture2D           gtxtNormal			: register(t1);
Texture2D           gtxtSpecPow	 		: register(t2);
Texture2D           gtxtDepth	 		: register(t3);
*/

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{

	//SURFACE_DATA gbd = UnpackGBuffer(input.texCoord);


	//------------------------------------- 지훈 -----------------------------------------------
	float4 cColor = gtxtColorSpecInt.Sample(gssSamplerState, input.texCoord);
	return float4(cColor.xyz, 1.0);
//	return(float4(gbd.Normal, 1.0f));

}