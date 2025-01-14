#include "SpotLightCollectionD3D11.h"

void SpotLightCollectionD3D11::Initialize(ID3D11Device* device, const SpotLightData& lightInfo)
{
	shadowMaps.Initialize(device, lightInfo.shadowMapInfo.textureDimension, 
		lightInfo.shadowMapInfo.textureDimension, true, 
		static_cast<UINT>(lightInfo.perLightInfo.size()));

	shadowCameras.reserve(lightInfo.perLightInfo.size());

	for (const auto& light : lightInfo.perLightInfo)
	{
		shadowCameras.push_back(CameraD3D11());
		ProjectionInfo projInfo = { light.angle * 2, 1, light.projectionNearZ, light.projectionFarZ };
		shadowCameras.back().Initialize(device, projInfo, light.initialPosition);
		shadowCameras.back().RotateUp(light.rotationY);
		shadowCameras.back().RotateRight(light.rotationX);

		LightBuffer bufferEntry = { shadowCameras.back().GetViewProjectionMatrix(), 
			light.colour, shadowCameras.back().GetForward(), light.angle,
			shadowCameras.back().GetPosition() };
		bufferData.push_back(bufferEntry);
	}

	lightBuffer.Initialize(device, sizeof(LightBuffer), 
		static_cast<UINT>(bufferData.size()), bufferData.data(), true);
}

void SpotLightCollectionD3D11::UpdateLightBuffers(ID3D11DeviceContext* context)
{
	for (auto& camera : shadowCameras)
	{
		camera.UpdateInternalConstantBuffer(context);
	}

	lightBuffer.UpdateBuffer(context, bufferData.data());
}

UINT SpotLightCollectionD3D11::GetNrOfLights() const
{
	return static_cast<UINT>(shadowCameras.size());
}

ID3D11DepthStencilView* SpotLightCollectionD3D11::GetShadowMapDSV(UINT lightIndex) const
{
	return shadowMaps.GetDSV(lightIndex);
}

ID3D11ShaderResourceView* SpotLightCollectionD3D11::GetShadowMapsSRV() const
{
	return shadowMaps.GetSRV();
}

ID3D11ShaderResourceView* SpotLightCollectionD3D11::GetLightBufferSRV() const
{
	return lightBuffer.GetSRV();
}

ID3D11Buffer* SpotLightCollectionD3D11::GetLightCameraConstantBuffer(UINT lightIndex) const
{
	return shadowCameras[lightIndex].GetConstantBuffer();
}
