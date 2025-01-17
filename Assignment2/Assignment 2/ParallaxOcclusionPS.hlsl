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
    float3 viewingVec = normalize(cameraPosition - input.worldPos);
    
    float3x3 tangentToWorld = float3x3(input.tangent, input.bitangent, input.normal);
    float3x3 inverseTangentToWorld = transpose(tangentToWorld);
    float3 viewTangent = normalize(mul(viewingVec, inverseTangentToWorld));
          
    float heightScale = 0.1f;
    
    float3 lengthFactor = dot(float3(0.0f, 0.0f, 1.0f), viewTangent);
     
    float3 sampleRay = (mul(-viewingVec, inverseTangentToWorld) / lengthFactor) * heightScale;
    
    int maxSampleCount = 32;
    float3 stepRate = sampleRay / maxSampleCount;
    
    float3 lastDisplacement = (0.0f, 0.0f, 0.0f);
    float3 currentDisplacement = (0.0f, 0.0f, 0.0f);
    
    for (uint j = 0; j < maxSampleCount; j++)
    {
        currentDisplacement = stepRate * j;
        float currentDepth = -currentDisplacement.z;
        float2 currentTextureCoords = input.textureCoords + currentDisplacement.xy;
        
        float3 sampledDepth = displacementMap.Sample(textureSampler, currentTextureCoords) * heightScale;
        
        if (currentDepth >= sampledDepth.z)
        {
            break;
        }
        
        lastDisplacement = currentDisplacement;
    }
    
    float2 beforeCoords = input.textureCoords + lastDisplacement.xy;
    float2 afterCoords = input.textureCoords + currentDisplacement.xy;
    float3 beforeDepth = displacementMap.Sample(textureSampler, beforeCoords) * heightScale;
    float3 afterDepth = displacementMap.Sample(textureSampler, afterCoords) * heightScale;
    
    float3 beforeDiff = -lastDisplacement.z - beforeDepth;
    float3 afterDiff = afterDepth - -currentDisplacement.z;
    float3 totalDiff = beforeDiff + afterDiff;
    
    float2 weight = clamp(beforeDiff / totalDiff, 0, 1);
    
    float2 weightCoord = (beforeCoords * weight) + (afterCoords * (1 - weight));
    
    
    float3 ambientMapValue = ambientMap.Sample(textureSampler, weightCoord);
    float3 diffuseMapValue = diffuseMap.Sample(textureSampler, weightCoord);
    float3 specularMapValue = specularMap.Sample(textureSampler, weightCoord);
    float3 normalMapValue = normalMap.Sample(textureSampler, weightCoord);
    float3 normal = normalize(normalMapValue * 2 - 1);
		
    normal = normalize(mul(normal, transpose(tangentToWorld)));
	
    float3 ambientValue = ambientMapValue;
    float3 diffuseValue = float3(0.0f, 0.0f, 0.0f);
    float3 specularValue = float3(0.0f, 0.0f, 0.0f);
	
    uint nrOfLights;
    uint stride;
    lights.GetDimensions(nrOfLights, stride);
	
	
    for (uint i = 0; i < nrOfLights; i++)
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
