#include "Quaternion.hlsli"

cbuffer ViewProjection {
	float4x4 gmtxViewProjection : register(b0);
};
struct VS_INS_OUTPUT {
//	float3 size : SIZE;
	float3 position : POSITION;
	float3 extend : EXTEND;
	float4 quaternion : QUATERNION;
};

struct GS_INS_OUTPUT {
	float4 position : SV_POSITION;
	float3 positionW : POSITION;

};

[maxvertexcount(24)]
void main(point VS_INS_OUTPUT input[1], inout LineStream<GS_INS_OUTPUT> stream) {
	GS_INS_OUTPUT v[8];

	//모델 좌표
	float3 position = input[0].position;

	float fExtendW = input[0].extend.x;
	float fExtendH = input[0].extend.y;
	float fExtendZ = input[0].extend.z;

	//월드 좌표 점 8개 미리 계산
	float3 positionW[8];
	//회전 + 이동
	positionW[0] = QuaternionRotation(float3(0 - fExtendW, 0 - fExtendH, 0 - fExtendZ), input[0].quaternion) + position;
	positionW[1] = QuaternionRotation(float3(0 - fExtendW, 0 + fExtendH, 0 - fExtendZ), input[0].quaternion) + position;
	positionW[2] = QuaternionRotation(float3(0 + fExtendW, 0 - fExtendH, 0 - fExtendZ), input[0].quaternion) + position;
	positionW[3] = QuaternionRotation(float3(0 + fExtendW, 0 + fExtendH, 0 - fExtendZ), input[0].quaternion) + position;
	positionW[4] = QuaternionRotation(float3(0 + fExtendW, 0 - fExtendH, 0 + fExtendZ), input[0].quaternion) + position;
	positionW[5] = QuaternionRotation(float3(0 + fExtendW, 0 + fExtendH, 0 + fExtendZ), input[0].quaternion) + position;
	positionW[6] = QuaternionRotation(float3(0 - fExtendW, 0 - fExtendH, 0 + fExtendZ), input[0].quaternion) + position;
	positionW[7] = QuaternionRotation(float3(0 - fExtendW, 0 + fExtendH, 0 + fExtendZ), input[0].quaternion) + position;
	
	///////////////////////////////11
	v[0].positionW = positionW[0];
	v[0].position = mul(float4(v[0].positionW, 1.0f), gmtxViewProjection);

	v[1].positionW = positionW[1];
	v[1].position = mul(float4(v[1].positionW, 1.0f), gmtxViewProjection);

	v[2].positionW = positionW[2];
	v[2].position = mul(float4(v[2].positionW, 1.0f), gmtxViewProjection);

	v[3].positionW = positionW[3];
	v[3].position = mul(float4(v[3].positionW, 1.0f), gmtxViewProjection);

	///////////////////////////////22
	v[4].positionW = positionW[4];
	v[4].position = mul(float4(v[4].positionW, 1.0f), gmtxViewProjection);

	v[5].positionW = positionW[5];
	v[5].position = mul(float4(v[5].positionW, 1.0f), gmtxViewProjection);

	v[6].positionW = positionW[6];
	v[6].position = mul(float4(v[6].positionW, 1.0f), gmtxViewProjection);

	v[7].positionW = positionW[7];
	v[7].position = mul(float4(v[7].positionW, 1.0f), gmtxViewProjection);

	//11
	stream.Append(v[1]);
	stream.Append(v[3]);
	stream.Append(v[5]);
	stream.Append(v[7]);

	//22
	stream.RestartStrip();
	stream.Append(v[0]);
	stream.Append(v[1]);
	stream.Append(v[7]);
	stream.Append(v[6]);
	
	stream.RestartStrip();
	stream.Append(v[2]);
	stream.Append(v[0]);
	stream.Append(v[6]);
	stream.Append(v[4]);
	
	stream.RestartStrip();
	stream.Append(v[3]);
	stream.Append(v[2]);
	stream.Append(v[4]);
	stream.Append(v[5]); 
}



