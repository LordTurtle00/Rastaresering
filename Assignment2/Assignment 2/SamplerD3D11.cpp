#include "SamplerD3D11.h"

#include <stdexcept>

SamplerD3D11::SamplerD3D11(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE adressMode,
	std::optional<std::array<float, 4>> borderColour)
{
	Initialize(device, adressMode, borderColour);
}

SamplerD3D11::~SamplerD3D11()
{
	if (sampler != nullptr)
		sampler->Release();
}

void SamplerD3D11::Initialize(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE adressMode, 
	std::optional<std::array<float, 4>> borderColour)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = adressMode;
	desc.AddressV = adressMode;
	desc.AddressW = adressMode;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 16;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	if (borderColour.has_value())
	{
		desc.BorderColor[0] = (*borderColour)[0];
		desc.BorderColor[1] = (*borderColour)[1];
		desc.BorderColor[2] = (*borderColour)[2];
		desc.BorderColor[3] = (*borderColour)[3];
	}
	else
	{
		desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0.0f;
	}

	HRESULT hr = device->CreateSamplerState(&desc, &sampler);
	
	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create sampler");
	}
}

ID3D11SamplerState* SamplerD3D11::GetSamplerState() const
{
	return sampler;
}
