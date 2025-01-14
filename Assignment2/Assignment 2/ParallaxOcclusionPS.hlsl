struct InputPS
{
	float4 svPos : SV_POSITION;
	float3 worldPos : WORLD_POSITION;
	float2 textureCoords : TEXTURE_COORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

cbuffer CameraDataCB : register(b0)
{
	float4x4 vp;
	float3 cameraPosition;
	float padding;
};

struct SpotlightData
{
	float4x4 vpMatrix;
	float3 colour;
	float3 direction;
	float angle;
	float3 position;
};

StructuredBuffer<SpotlightData> lights : register(t0);
Texture2D<float4> ambientMap : register(t1);
Texture2D<float4> diffuseMap : register(t2);
Texture2D<float4> specularMap : register(t3);
Texture2D<float4> normalMap : register(t4);
Texture2D<float4> displacementMap : register(t5);

sampler textureSampler : register(s0);

float4 main(InputPS input) : SV_TARGET
{

}