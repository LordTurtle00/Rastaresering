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
	float3 ambientMapValue = ambientMap.Sample(textureSampler, input.textureCoords);
	float3 diffuseMapValue = diffuseMap.Sample(textureSampler, input.textureCoords);
    float3 specularMapValue = specularMap.Sample(textureSampler, input.textureCoords);
    float3 normalMapValue = normalMap.Sample(textureSampler, input.textureCoords);
    float3 normal = normalize(normalMapValue * 2 - 1);
	
	
    float3 viewingVec = normalize(cameraPosition - input.worldPos);
    float3x3 worldToTangent = float3x3(input.bitangent, input.tangent, input.normal);
	
    float3 viewTangent = normalize(mul(viewingVec, transpose(worldToTangent)));
	
    normal = normalize(mul(normal, transpose(worldToTangent)));
	
    float3 ambientValue = ambientMapValue;
    float3 diffuseValue = float3(0.0f, 0.0f, 0.0f);
    float3 specularValue = float3(0.0f, 0.0f, 0.0f);
	
    uint nrOfLights;
    uint stride;
    lights.GetDimensions(nrOfLights, stride);
	
	
    for (uint i = 0; i < nrOfLights; ++i)
    {
        SpotlightData light = lights[i];
		
        float3 lightVec = normalize(light.position - input.worldPos);
        float diffuse = max(dot(normal, lightVec), 0.0);
        float3 reflectiv = reflect(-lightVec, normal);
        float specular = pow(max(dot(viewingVec, reflectiv), 0.0), 8);
		
        diffuseValue += diffuse * light.colour;
        specularValue += specular * light.colour;
						
    }
    
    diffuseValue *= diffuseMapValue;
    specularValue *= specularMapValue;
   	
    float3 outColor = float3(ambientValue + diffuseValue + specularValue);
    return float4(outColor, 1.0f);
}