cbuffer CBuffer
{
	matrix mtxWorld;
	matrix mtxView;
	matrix mtxProjection;
};
struct Vertex
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;  // �ؼ� ��ǥ�� UV��
};
struct Pixel
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};
Pixel TextureVertexShader(Vertex input)
{
	Pixel output;

	//��position
	input.position.w  = 1.f;
	output.position   = mul(input.position,  mtxWorld);
	output.position   = mul(output.position, mtxView);
	output.position   = mul(output.position, mtxProjection);

	//��texcoord
	output.texcoord   = input.texcoord;

	//��return
	return output;
}