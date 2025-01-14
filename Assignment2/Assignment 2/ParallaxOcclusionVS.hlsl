cbuffer CameraDataCB : register(b0)
{
	float4x4 vp;
	float3 cameraPosition;
	float padding;
};

cbuffer Transform : register(b1)
{
	float4x4 worldMatrix;
};

struct InputVS
{
	float3 position : POSITION;
	float2 textureCoords : TEXTURE_COORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct OutputVS
{
	float4 svPos : SV_POSITION;
	float3 worldPos : WORLD_POSITION;
	float2 textureCoords : TEXTURE_COORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

OutputVS main(InputVS input)
{
	OutputVS toOutput;
	
    toOutput.worldPos = mul(float4(input.position, 1), worldMatrix);
    toOutput.svPos = mul(float4(toOutput.worldPos, 1) ,vp);
    toOutput.textureCoords = input.textureCoords;
    toOutput.normal = normalize(mul(float4(input.normal, 1), worldMatrix));
    toOutput.tangent = mul(float4(input.tangent, 1), worldMatrix);
    toOutput.bitangent = mul(float4(input.bitangent, 1), worldMatrix);

	return toOutput;
}