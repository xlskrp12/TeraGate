Texture2D    texture2d;
SamplerState samplerState;
struct Pixel
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};
float4 TexturePixelShader(Pixel input) : SV_TARGET
{
	float4 output;
	output = texture2d.Sample(samplerState, input.texcoord); // �ؼ� ���÷��� �ؽ��ķκ��� �ȼ� ������ ���ø�.
	return output;
}