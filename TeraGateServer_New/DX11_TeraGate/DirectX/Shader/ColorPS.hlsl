// ≈∏¿‘¡§¿«
struct Pixel
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
};
// «»ºø ºŒ¿Ã¥ı
float4 ColorPixelShader(Pixel input) : SV_TARGET
{
	return input.color;
}