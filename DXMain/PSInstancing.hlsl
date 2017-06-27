

// 픽셀 셰이더를 통과한 픽셀당 색 데이터입니다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

// (보간된) 색 데이터에 대한 통과 함수입니다.
float4 main(VS_OUTPUT input) : SV_TARGET
{
	return input.color;
	//return float4(0,0,1,1);
}
