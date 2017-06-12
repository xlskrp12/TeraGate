// 전역
// ColorShader클래스에 선언할 구조체와 형태가 동일해야 된다.
cbuffer CBuffer
{
	matrix mtxWorld;
	matrix mtxView;
	matrix mtxProjection;
};
// 타입정의
struct Vertex
{
	float4 position : POSITION;
	float4 color    : COLOR;
};
struct Pixel
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
};
// 버텍스 셰이더
Pixel ColorVertexShader(Vertex input)
{
	Pixel output;

	// 정점을 월드, 뷰, 사영 순으로 계산
	input.position.w = 1.0f;
	// 정점위치를 월드&뷰&사영 순으로 계산
	output.position = mul(input.position, mtxWorld);
	output.position = mul(output.position, mtxView);
	output.position = mul(output.position, mtxProjection);
	// 입력 색상 저장
	output.color = input.color;

	return output;
}