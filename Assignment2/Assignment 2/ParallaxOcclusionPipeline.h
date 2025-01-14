#pragma once

#include <vector>

#include "ShaderD3D11.h"
#include "InputLayoutD3D11.h"
#include "SamplerD3D11.h"
#include "RenderableObject.h"
#include "CameraD3D11.h"
#include "SpotLightCollectionD3D11.h"
#include "DepthBufferD3D11.h"

class ParallaxOcclusionPipeline
{
private:
	ID3D11Device* device;
	D3D11_VIEWPORT viewPort;

	ShaderD3D11 vertexShader;
	ShaderD3D11 pixelShader;
	InputLayoutD3D11 inputLayout;
	SamplerD3D11 sampler;

public:
	ParallaxOcclusionPipeline() = default;
	~ParallaxOcclusionPipeline() = default;
	ParallaxOcclusionPipeline(const ParallaxOcclusionPipeline& other) = delete;
	ParallaxOcclusionPipeline& operator=(const ParallaxOcclusionPipeline& other) = delete;
	ParallaxOcclusionPipeline(ParallaxOcclusionPipeline&& other) = delete;
	ParallaxOcclusionPipeline& operator=(ParallaxOcclusionPipeline&& other) = delete;

	void Initialize(ID3D11Device* deviceToUse, float windowWidth, 
		float windowHeight);

	void Render(const std::vector<RenderableObject>& objects,
		const CameraD3D11& camera, const SpotLightCollectionD3D11& lights,
		ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv,
		DepthBufferD3D11& depthBuffer);
};