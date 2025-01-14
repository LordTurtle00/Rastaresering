#include "ShaderD3D11.h"

#include <stdexcept>
#include <fstream>

#include <d3dcompiler.h>

ShaderD3D11::~ShaderD3D11()
{
	if (shader.vertex != nullptr)
	{
		switch (type)
		{
		case ShaderType::VERTEX_SHADER:
			shader.vertex->Release();
			break;
		case ShaderType::HULL_SHADER:
			shader.hull->Release();
			break;
		case ShaderType::DOMAIN_SHADER:
			shader.domain->Release();
			break;
		case ShaderType::GEOMETRY_SHADER:
			shader.geometry->Release();
			break;
		case ShaderType::PIXEL_SHADER:
			shader.pixel->Release();
			break;
		case ShaderType::COMPUTE_SHADER:
			shader.compute->Release();
			break;
		default:
			break;
		}

		shaderBlob->Release();
	}
}

ShaderD3D11::ShaderD3D11(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize)
{
	Initialize(device, shaderType, dataPtr, dataSize);
}

ShaderD3D11::ShaderD3D11(ID3D11Device* device, ShaderType shaderType, const char* csoPath)
{
	Initialize(device, shaderType, csoPath);
}

void ShaderD3D11::Initialize(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize)
{
	type = shaderType;
	D3DCreateBlob(dataSize, &shaderBlob);
	memcpy(shaderBlob->GetBufferPointer(), dataPtr, dataSize);
	HRESULT hr = S_OK;

	switch (type)
	{
	case ShaderType::VERTEX_SHADER:
		hr = device->CreateVertexShader(dataPtr, dataSize, nullptr, &shader.vertex);
		break;
	case ShaderType::HULL_SHADER:
		hr = device->CreateHullShader(dataPtr, dataSize, nullptr, &shader.hull);
		break;
	case ShaderType::DOMAIN_SHADER:
		hr = device->CreateDomainShader(dataPtr, dataSize, nullptr, &shader.domain);
		break;
	case ShaderType::GEOMETRY_SHADER:
		hr = device->CreateGeometryShader(dataPtr, dataSize, nullptr, &shader.geometry);
		break;
	case ShaderType::PIXEL_SHADER:
		hr = device->CreatePixelShader(dataPtr, dataSize, nullptr, &shader.pixel);
		break;
	case ShaderType::COMPUTE_SHADER:
		hr = device->CreateComputeShader(dataPtr, dataSize, nullptr, &shader.compute);
		break;
	default:
		throw std::runtime_error("Unknown type of shader when initializing it");
		break;
	}

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create shader");
	}
}

void ShaderD3D11::Initialize(ID3D11Device* device, ShaderType shaderType, const char* csoPath)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open(csoPath, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		throw std::runtime_error("Could not open CSO file");
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	Initialize(device, shaderType, shaderData.c_str(), shaderData.size());
}

const void* ShaderD3D11::GetShaderByteData() const
{
	return shaderBlob->GetBufferPointer();
}

size_t ShaderD3D11::GetShaderByteSize() const
{
	return shaderBlob->GetBufferSize();
}

void ShaderD3D11::BindShader(ID3D11DeviceContext* context) const
{
	switch (type)
	{
	case ShaderType::VERTEX_SHADER:
		context->VSSetShader(shader.vertex, nullptr, 0);
		break;
	case ShaderType::HULL_SHADER:
		context->HSSetShader(shader.hull, nullptr, 0);
		break;
	case ShaderType::DOMAIN_SHADER:
		context->DSSetShader(shader.domain, nullptr, 0);
		break;
	case ShaderType::GEOMETRY_SHADER:
		context->GSSetShader(shader.geometry, nullptr, 0);
		break;
	case ShaderType::PIXEL_SHADER:
		context->PSSetShader(shader.pixel, nullptr, 0);
		break;
	case ShaderType::COMPUTE_SHADER:
		context->CSSetShader(shader.compute, nullptr, 0);
		break;
	default:
		throw std::runtime_error("Unknown type of shader when binding it");
		break;
	}
}
