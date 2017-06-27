#define MAX_SPOT_LIGHT 800

cbuffer CameraPosition : register(b1) {
	float4x4 CameraProjectionMtx:packoffset(c0);
	float4 EyePosition:packoffset(c4);
}

struct SpotLight {
	float3 SpotLightPos;
	float SpotLightRangeRcp;

	float3 SpotLightDir;
	float SpotCosOuterCone;

	float3 SpotLightColor;
	float SpotCosConeAttRcp;
};

cbuffer cbSpotLightDomain : register(b2) {
	SpotLight gSpotLight[MAX_SPOT_LIGHT];
}

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};


float3 CalcSpot(float3 position, Material material, uint instanceID) {
	
	float3 ToLight = gSpotLight[instanceID].SpotLightPos - position;
	float3 ToEye = EyePosition.xyz - position;
	float DistToLight = length(ToLight);

	//Æþ µðÇ»Áî
	ToLight /= DistToLight; //Á¤±ÔÈ­
	float NDotL = saturate(dot(ToLight, material.normal));
	float3 finalColor = gSpotLight[instanceID].SpotLightColor.rgb * NDotL;

	//ºí¸° ½ºÆåÅ§·¯
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += gSpotLight[instanceID].SpotLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;
	//return finalColor;

	float cosAng = dot(gSpotLight[instanceID].SpotLightDir, ToLight);
	//¿ø»Ô °¨¼â
	float conAtt = saturate((cosAng - gSpotLight[instanceID].SpotCosOuterCone) * gSpotLight[instanceID].SpotCosConeAttRcp);
	conAtt *= conAtt;

	//°¨¼â
	float DistToLightNorm = 1.0 - saturate(DistToLight * gSpotLight[instanceID].SpotLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= material.diffuseColor.xyz * Attn * conAtt;

	return finalColor;
	//return finalColor;

}