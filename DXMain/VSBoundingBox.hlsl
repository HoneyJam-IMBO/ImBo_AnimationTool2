////struct VS_INPUT {
////	float3 positionW : INSTANCEPOS;
////	float3 extend  : EXTEND;
////};
//
//struct VS_OUTPUT {
//	uint instance : TEXCOORD;
//	//float3 extend : TEXCOORD0;
//};
//
//VS_OUTPUT main( uint instance : SV_InstanceID)
//{
//	VS_OUTPUT output = (VS_OUTPUT)0;
//	//월드 변환도 필요없다. 그냥 보내줌
//	output.instance = instance;
//
//	return output;
//}

struct VS_INS_INPUT{
	//float3 size : SIZE;
	float3 position : INSTANCEPOS;
	float3 extend : EXTEND;
	float4 quaternion : QUATERNION;
};

struct VS_INS_OUTPUT{
	//float3 size : SIZE;
	float3 position : POSITION;
	float3 extend : EXTEND;
	float4 quaternion : QUATERNION;
};

VS_INS_OUTPUT main(VS_INS_INPUT input)
{
	//월드 좌표
	//float4 position = float4(0, 0, 0, 1.0f);


	VS_INS_OUTPUT output = (VS_INS_OUTPUT)0;
	//output.size = input.size;
	output.position = input.position;
	output.extend = input.extend;
	output.quaternion = input.quaternion;
	//output.positionW = position.xyz;


	//입력되는 정점의 size을 그대로 출력한다. 
	//output.color = input.color;
	//입력되는 정점의 색상을 그대로 출력한다. 
	return output;
}

