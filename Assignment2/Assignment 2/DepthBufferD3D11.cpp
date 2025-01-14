#include "DepthBufferD3D11.h"

#include <stdexcept>

DepthBufferD3D11::DepthBufferD3D11(ID3D11Device* device, UINT width, UINT height, bool hasSRV)
{
	Initialize(device, width, height, hasSRV);
}

DepthBufferD3D11::~DepthBufferD3D11()
{
	if (texture != nullptr)
		texture->Release();

	for (auto& dsv : depthStencilViews)
		dsv->Release();

	if (srv != nullptr)
		srv->Release();
}

void DepthBufferD3D11::Initialize(ID3D11Device* device, UINT width, UINT height,
	bool hasSRV, UINT arraySize)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = arraySize;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	desc.BindFlags |= hasSRV ? D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE : 0;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);

	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create depth buffer texture");
	}

	depthStencilViews.resize(arraySize);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 1;

	for (UINT dsvIndex = 0; dsvIndex < arraySize; ++dsvIndex)
	{
		dsvDesc.Texture2DArray.FirstArraySlice = dsvIndex;
		hr = device->CreateDepthStencilView(texture, &dsvDesc, &depthStencilViews[dsvIndex]);

		if (FAILED(hr))
		{
			throw std::runtime_error("Could not create depth buffer dsv");
		}
	}

	if (hasSRV == true)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.MipLevels = 1;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.ArraySize = arraySize;

		hr = device->CreateShaderResourceView(texture, &srvDesc, &srv);

		if (FAILED(hr))
		{
			throw std::runtime_error("Could not create depth buffer srv");
		}
	}

}

ID3D11DepthStencilView* DepthBufferD3D11::GetDSV(UINT arrayIndex) const
{
	return depthStencilViews[arrayIndex];
}

ID3D11ShaderResourceView* DepthBufferD3D11::GetSRV() const
{
	return srv;
}
