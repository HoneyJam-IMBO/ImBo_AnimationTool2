#define MAX_POINT_LIGHT 800

cbuffer CameraPosition : register(b1) {
	float4x4 CameraProjectionMtx:packoffset(c0);
	float4 EyePosition:packoffset(c4);
}

struct PointLight {
	float3 PointLightPos;
	float PointLightRange;
	float3 PointLightColor;
	float PointLightIntensity;
};

cbuffer cbPointLight : register(b2) {
	PointLight gPointLight[MAX_POINT_LIGHT];
}


struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

float3 CalcPoint(float3 position, Material material, uint instanceID) {
	float3 ToLight = gPointLight[instanceID].PointLightPos - position;
	float3 ToEye = EyePosition.xyz - position;
	float DistToLight = length(ToLight);

	ToLight /= DistToLight; //Normalize
	float NDotL = saturate(dot(ToLight, material.normal));
	float3 finalColor = gPointLight[instanceID].PointLightColor.rgb * NDotL;


	//∫Ì∏∞ Ω∫∆Â≈ß∑Ø
	ToEye = normalize(ToEye);
	//float3 HalfWay = normalize(ToEye + ToLight);
	//float3 vReflect = reflect(-ToLight, material.normal);
	//float NDotH = max(dot(vReflect, ToEye), 0.0f);
	//finalColor += gPointLight[instanceID].PointLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;
	//finalColor += pow(NDotH, material.specExp) * material.specIntensity;

	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += gPointLight[instanceID].PointLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	//∞®º‚
	float DistToLightNorm = 1.0 - saturate(DistToLight * gPointLight[instanceID].PointLightRange);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= material.diffuseColor.xyz * Attn;

	return finalColor;
}