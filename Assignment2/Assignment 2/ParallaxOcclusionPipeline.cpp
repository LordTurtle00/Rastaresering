#include "ParallaxOcclusionPipeline.h"

void ParallaxOcclusionPipeline::Initialize(ID3D11Device* deviceToUse, 
	float windowWidth, float windowHeight)
{
	device = deviceToUse;

	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = windowWidth;
	viewPort.Height = windowHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	vertexShader.Initialize(device, ShaderType::VERTEX_SHADER,
		"ParallaxOcclusionVS.cso");
	pixelShader.Initialize(device, ShaderType::PIXEL_SHADER,
		"ParallaxOcclusionPS.cso");

	inputLayout.AddInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayout.AddInputElement("TEXTURE_COORDS", DXGI_FORMAT_R32G32_FLOAT);
	inputLayout.AddInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayout.AddInputElement("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayout.AddInputElement("BITANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayout.FinalizeInputLayout(device, vertexShader.GetShaderByteData(),
		vertexShader.GetShaderByteSize());

	sampler.Initialize(device, D3D11_TEXTURE_ADDRESS_WRAP);
}

void ParallaxOcclusionPipeline::Render(const std::vector<RenderableObject>& objects,
	const CameraD3D11& camera, const SpotLightCollectionD3D11& lights, 
	ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, 
	DepthBufferD3D11& depthBuffer)
{
	context->IASetInputLayout(inputLayout.GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vertexShader.BindShader(context);
	ID3D11Buffer* cameraBuffer[] = { camera.GetConstantBuffer() };
	context->VSSetConstantBuffers(0, ARRAYSIZE(cameraBuffer), cameraBuffer);

	context->RSSetViewports(1, &viewPort);

	pixelShader.BindShader(context);
	context->PSSetConstantBuffers(0, ARRAYSIZE(cameraBuffer), cameraBuffer);
	ID3D11ShaderResourceView* lightSRV[] = { lights.GetLightBufferSRV() };
	context->PSSetShaderResources(0, ARRAYSIZE(lightSRV), lightSRV);
	ID3D11SamplerState* samplers[] = { sampler.GetSamplerState() };
	context->PSSetSamplers(0, ARRAYSIZE(samplers), samplers);

	context->OMSetRenderTargets(1, &rtv, depthBuffer.GetDSV(0));

	for (const RenderableObject& object : objects)
	{
		ID3D11Buffer* vertexBuffers[] = { object.vertexBuffer.GetBuffer() };
		UINT stride = object.vertexBuffer.GetVertexSize();
		UINT offset = 0;
		context->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers,
			&stride, &offset);
		context->IASetIndexBuffer(object.indexBuffer.GetBuffer(),
			DXGI_FORMAT_R32_UINT, 0);

		ID3D11Buffer* transformBuffer[] = { object.transformBuffer.GetBuffer() };
		context->VSSetConstantBuffers(1, ARRAYSIZE(transformBuffer), transformBuffer);

		ID3D11ShaderResourceView* materials[] = { object.ambientMap.GetSRV(), 
			object.diffuseMap.GetSRV(), object.specularMap.GetSRV(),
			object.normalMap.GetSRV(), object.displacementMap.GetSRV() };
		context->PSSetShaderResources(1, ARRAYSIZE(materials), materials);

		context->DrawIndexed(static_cast<UINT>(object.indexBuffer.GetNrOfIndices()), 0, 0);
	}
}
