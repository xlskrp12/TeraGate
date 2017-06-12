Texture2D    texture2d;
SamplerState samplerState;
cbuffer CBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float  padding;       // CreateBuffer()시 바이트 수를 16의 배수로 맞춰주기 위한 패딩. (사실상 가비지 값)
};
struct Pixel
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal   : NORMAL;
};
float4 LightPixelShader(Pixel input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float  lightIntensity;// 조도, 빛의 세기
	float4 color;

	//▼textureColor, lightIntensity
	textureColor   = texture2d.Sample(samplerState, input.texcoord);// sampler를 사용해 texcoord로부터 컬러 값 샘플
	lightDir       = -lightDirection;                               // 빛의 방향을 계산하기 위해 도치
	lightIntensity = saturate(dot(input.normal, lightDir));// 픽셀에 대한 조도(light intensity) 계산
                                                           // saturate(x) : x를 [0, 1] 범위로 클램프
	                                                       // dot(v1, v2) : 두 벡터 v1, v2 내적값 리턴
	color = saturate(diffuseColor * lightIntensity);       // 기본 확산색상(diffuse color)에 조도(light intensity)를 더해 최종 확산색상(diffuse color) 결정.
	color = color*textureColor;
    //▼result
	return color;
}