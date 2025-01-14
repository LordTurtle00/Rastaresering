#include "RenderTargetD3D11.h"

#include <stdexcept>

RenderTargetD3D11::~RenderTargetD3D11()
{
	if (texture != nullptr)
		texture->Release();

	if (rtv != nullptr)
		rtv->Release();

	if (srv != nullptr)
		srv->Release();
}

void RenderTargetD3D11::Initialize(ID3D11Device* device, 
	UINT width, UINT height, DXGI_FORMAT format, bool hasSRV)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	desc.BindFlags |= hasSRV ? D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE : 0;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);

	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create render target texture");
	}

	hr = device->CreateRenderTargetView(texture, nullptr, &rtv);

	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create render target rtv");
	}

	if (hasSRV == true)
	{
		device->CreateShaderResourceView(texture, nullptr, &srv);

		if (FAILED(hr))
		{
			throw std::runtime_error("Could not create render target srv");
		}
	}
}

ID3D11RenderTargetView* RenderTargetD3D11::GetRTV() const
{
	return rtv;
}

ID3D11ShaderResourceView* RenderTargetD3D11::GetSRV() const
{
	return srv;
}
