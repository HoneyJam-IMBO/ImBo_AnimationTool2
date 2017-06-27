

//ambient
cbuffer CameraPosition : register(b1) {
	float4x4 CameraProjectionMtx:packoffset(c0);
	float4 EyePosition:packoffset(c4);
}
//ambient
//directional
cbuffer Dir_AmbLightConstants : register(b2) {
	float4 DirToLight:packoffset(c0);
	float4 DirLightColor:packoffset(c1);
	float4 DirLightPower:packoffset(c2);

	float4 AmbientDown : packoffset(c3);
	float4 AmbientRange : packoffset(c4);
	float4 AmbientUp : packoffset(c5);
	float4 AmbientColor : packoffset(c6);
}
cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

//directional
float3 CalcDirectional(float3 position, Material material) {
	//퐁 디퓨즈
	float4 dirToLight = normalize(DirToLight);
	float NDotL = dot(dirToLight.xyz, material.normal);
	float3 finalColor = DirLightColor.rgb * saturate(NDotL);

	//블린 스펙큘러
	float3 ToEye = EyePosition.xyz - position;
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + DirToLight.xyz);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += DirLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	return finalColor * material.diffuseColor.rgb;
}
//directional
//ambient
float3 CalcAmbient(float3 normal, float3 color) {
	//[-1, 1]에서 [0, 1]으로 변환
	float up = normal.y * 0.5 + 0.5;

	//ambient값 계산
	float3 Ambient = AmbientDown.xyz + up * AmbientUp.xyz;

	//색상 값에 엠비언트 값 적용
	return Ambient * color;
}
//ambient
