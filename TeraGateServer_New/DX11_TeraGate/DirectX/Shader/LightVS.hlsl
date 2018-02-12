cbuffer CBuffer
{
	matrix mtxWorld;
	matrix mtxView;
	matrix mtxProjection;
};
struct Vertex
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal   : NORMAL;
};
struct Pixel
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal   : NORMAL;       // ��ֺ���, �������� : ��鿡 ������ ����
};
Pixel LightVertexShader(Vertex input)
{
	Pixel output;
	
	//��position
	input.position.w = 1.f;
	output.position = mul(input.position, mtxWorld);
	output.position = mul(output.position, mtxView);
	output.position = mul(output.position, mtxProjection);

	//��texcoord
	output.texcoord = input.texcoord;

	//��normal
	output.normal = mul(input.normal, (float3x3)mtxWorld);// �������ʹ� ������ǥ�迡�� ����.
	output.normal = normalize(output.normal);	          // �ȼ����̴��� ���� ���� ����ȭ(normalize)

	// return
	return output;
}