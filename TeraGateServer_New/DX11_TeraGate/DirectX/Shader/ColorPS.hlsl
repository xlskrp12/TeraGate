// Ÿ������
struct Pixel
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
};
// �ȼ� ���̴�
float4 ColorPixelShader(Pixel input) : SV_TARGET
{
	return input.color;
}