#define MAX_CAPSULE_LIGHT 800

cbuffer CameraPosition : register(b1) {
	float4x4 CameraProjectionMtx:packoffset(c0);
	float4 EyePosition:packoffset(c4);
}

struct CapsuleLight {
	float3 CapsuleLightPos;
	float CapsuleLightRangeRcp;

	float3 CapsuleLightDir;
	float CapsuleLightLen;

	float3 CapsuleLightColor;
	float CapsuleLightIntensity;
};

cbuffer cbCapsuleLightDomain : register(b2) {
	CapsuleLight gCapsuleLight[MAX_CAPSULE_LIGHT];
}

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

float3 CalcCapsule(float3 position, Material material, uint instanceID) {
	float3 ToEye = EyePosition.xyz - position;

	//return gCapsuleLight[instanceID].CapsuleLightColor;
	//ÇÈ¼¿°ú Ä¸½¶ ¼±ºÐ »çÀÌÀÇ ÃÖ¼Ò°Å¸® ±¸ÇÏ±â
	float3 ToCapsuleStart = position - gCapsuleLight[instanceID].CapsuleLightPos;
	float DistOnLine = dot(ToCapsuleStart, gCapsuleLight[instanceID].CapsuleLightDir) / gCapsuleLight[instanceID].CapsuleLightLen;
	DistOnLine = saturate(DistOnLine) * gCapsuleLight[instanceID].CapsuleLightLen;
	float3 PointOnLine = gCapsuleLight[instanceID].CapsuleLightPos 
		+ gCapsuleLight[instanceID].CapsuleLightDir * DistOnLine;
	float3 ToLight = PointOnLine - position;
	float DistToLight = length(ToLight);

	//Æþ µðÇ»Áî
	ToLight /= DistToLight; //Normalize
	float NDotL = saturate(dot(ToLight, material.normal));
	float3 finalColor = material.diffuseColor.xyz * NDotL;
	
	//ºí¸° ½ºÆåÅ§·¯
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += pow(NDotH, material.specExp) * material.specIntensity;

	//°¨¼â
	float DistToLightNorm = 1.0 - saturate(DistToLight * gCapsuleLight[instanceID].CapsuleLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	//finalColor *= gCapsuleLight[instanceID].CapsuleLightColor.rgb 
	//	* gCapsuleLight[instanceID].CapsuleLightIntensity * Attn;

	finalColor *= gCapsuleLight[instanceID].CapsuleLightColor.rgb * Attn;
	//finalColor *= gCapsuleLight[instanceID].CapsuleLightColor.rgb
	//	* Attn;
	//float intensity = gCapsuleLight[instanceID].CapsuleLightIntensity;
	//return gCapsuleLight[instanceID].CapsuleLightDir;

	//return gCapsuleLight[instanceID].CapsuleLightColor;
	//return float3(intensity, intensity, intensity);

	return finalColor;

}