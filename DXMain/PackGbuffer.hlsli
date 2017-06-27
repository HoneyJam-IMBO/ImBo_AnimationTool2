

//specpowr 정규화 
static const float2 g_SpecPowerRange = { 0.1, 250.0 };


struct PS_GBUFFER_OUT {
	float4 ColorSpecInt : SV_TARGET0;
	float4 Normal_Depth : SV_TARGET1;
	float4 PositionW_SpecPow : SV_TARGET2;	
};

PS_GBUFFER_OUT PackGBuffer(float3 BaseColor, float3 Normal, float3 PositionW, float3 SpecIntensity, float SpecPower, float Depth) {
	PS_GBUFFER_OUT Out = (PS_GBUFFER_OUT)0;

	//스펙큘러 파워 정규화
	float SpecPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;

	//GBuffer 구조체에 데이터 패킹
	Out.ColorSpecInt = float4(BaseColor.rgb, 0.f);
	Out.Normal_Depth = float4(Normal.xyz * 0.5 + 0.5, Depth);
	//Out.Normal_Depth = float4(Normal.xyz, Depth);

	//positionW/ specPow값 패킹
	Out.PositionW_SpecPow.w = SpecPowerNorm;
	Out.PositionW_SpecPow.xyz = SpecIntensity;

	return Out;
}