struct VS_OUTPUT {
	float4 position : SV_POSITION;
	uint instanceID : TEXCOORD0;
};

VS_OUTPUT main(uint instanceID : SV_InstanceID)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

output.position = float4(0.0, 0.0, 0.0, 1.0);
output.instanceID = instanceID;

	return output;
}