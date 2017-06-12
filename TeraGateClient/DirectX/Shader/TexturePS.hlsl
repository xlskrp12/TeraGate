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
	output = texture2d.Sample(samplerState, input.texcoord); // 텍셀 샘플러로 텍스쳐로부터 픽셀 색상을 샘플링.
	return output;
}