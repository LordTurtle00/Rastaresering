#include "ShaderResourceTextureD3D11.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ID3D11Device* device, UINT width, UINT height, void* textureData)
{
	Initialize(device, width, height, textureData);
}

ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ID3D11Device* device, const char* pathToTextureFile)
{
	Initialize(device, pathToTextureFile);
}

ShaderResourceTextureD3D11::~ShaderResourceTextureD3D11()
{
	if (texture != nullptr)
		texture->Release();

	if (srv != nullptr)
		srv->Release();
}

ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ShaderResourceTextureD3D11&& other) : 
	texture(other.texture), srv(other.srv)
{
	other.texture = nullptr;
	other.srv = nullptr;
}

ShaderResourceTextureD3D11& ShaderResourceTextureD3D11::operator=(ShaderResourceTextureD3D11&& other)
{
	if (this != &other)
	{
		texture = other.texture;
		srv = other.srv;

		other.texture = nullptr;
		other.srv = nullptr;
	}

	return *this;
}

void ShaderResourceTextureD3D11::Initialize(ID3D11Device* device, UINT width, UINT height, void* textureData)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = textureData;
	data.SysMemPitch = width * 4;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateTexture2D(&desc, &data, &texture);

	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create shader resource texture");
	}

	hr = device->CreateShaderResourceView(texture, nullptr, &srv);

	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create shader resource texture srv");
	}
}

void ShaderResourceTextureD3D11::Initialize(ID3D11Device* device, const char* pathToTextureFile)
{
	int width;
	int height;
	unsigned char* image = stbi_load(pathToTextureFile, &width, &height, nullptr, 4);

	Initialize(device, static_cast<UINT>(width), static_cast<UINT>(height), image);

	stbi_image_free(image);
}

ID3D11ShaderResourceView* ShaderResourceTextureD3D11::GetSRV() const
{
	return srv;
}
