Texture2D    texture2d;
SamplerState samplerState;
cbuffer CBuffer
{
    float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float  padding;       // CreateBuffer()�� ����Ʈ ���� 16�� ����� �����ֱ� ���� �е�. (��ǻ� ������ ��)
};
struct Pixel
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal   : NORMAL;
};
float4 AmbientLightPS(Pixel input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float  lightIntensity;// ����, ���� ����
	float4 color;

	//��textureColor, lightIntensity
	textureColor   = texture2d.Sample(samplerState, input.texcoord);// sampler�� ����� texcoord�κ��� �÷� �� ����
    color          = ambientColor;                                  // �⺻ ������ �ֺ��� ������. ��� �ȼ��� �ּ��� �ֺ��� ���� ������ �ȴ�.
	lightDir       = -lightDirection;                               // ���� ������ ����ϱ� ���� ��ġ
    //���ȼ��� ���� ����(light intensity) ���
    // saturate(x) : x�� [0, 1] ������ Ŭ����
    // dot(v1, v2) : �� ���� v1, v2 ������ ����
    lightIntensity = saturate(dot(input.normal, lightDir));
    // ����, ���� ���� ���� 0���� ū��?
    // ũ�ٸ� �������� �ֺ����� ���ϰ� �׷��� ������ �������� �ȴ��Ѵ�.
    // ��Ȥ ������ ���Ǵ� ���� ��� ���� �ֺ��� ���� ��� �Դ� ��츦 ����
    if(0.0f < lightIntensity)
        color += (diffuseColor * lightIntensity);// �⺻ Ȯ�����(diffuse color)�� ����(light intensity)�� ���� ���� Ȯ�����(diffuse color) ����.
	color = saturate(color);                     // �ֺ���+���� ���� 1�� ���� �� �����Ƿ� saturate()�� 0~1���� ������ ����
	color = color*textureColor;
    //��result
	return color;
}