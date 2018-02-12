// ����
// ColorShaderŬ������ ������ ����ü�� ���°� �����ؾ� �ȴ�.
cbuffer CBuffer
{
	matrix mtxWorld;
	matrix mtxView;
	matrix mtxProjection;
};
// Ÿ������
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
// ���ؽ� ���̴�
Pixel ColorVertexShader(Vertex input)
{
	Pixel output;

	// ������ ����, ��, �翵 ������ ���
	input.position.w = 1.0f;
	// ������ġ�� ����&��&�翵 ������ ���
	output.position = mul(input.position, mtxWorld);
	output.position = mul(output.position, mtxView);
	output.position = mul(output.position, mtxProjection);
	// �Է� ���� ����
	output.color = input.color;

	return output;
}