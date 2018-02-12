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
	float3 normal   : NORMAL;       // 노멀벡터, 법선벡터 : 평면에 수직인 벡터
};
Pixel LightVertexShader(Vertex input)
{
	Pixel output;
	
	//▼position
	input.position.w = 1.f;
	output.position = mul(input.position, mtxWorld);
	output.position = mul(output.position, mtxView);
	output.position = mul(output.position, mtxProjection);

	//▼texcoord
	output.texcoord = input.texcoord;

	//▼normal
	output.normal = mul(input.normal, (float3x3)mtxWorld);// 법선벡터는 월드좌표계에서 계산됨.
	output.normal = normalize(output.normal);	          // 픽셀셰이더에 들어가기 전에 정규화(normalize)

	// return
	return output;
}