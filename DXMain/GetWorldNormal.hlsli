
texture2D gtxtNormal : register(t5);
sampler gssNormal : register(s5);


float3 GetWorldNormal(float3 tangentW, float3 bitangentW, float2 texCoord) {
	float3 T = normalize(tangentW);
	float3 B = normalize(bitangentW);
	float3 N = cross(T, B);
	float3x3 TBN = float3x3(T, B, N);

	//구한 TBN을 Normal Map에서 얻은 Normal에 곱한다.
	float3 normal = gtxtNormal.Sample(gssNormal, texCoord).xyz;
	return mul(normal, TBN);
}