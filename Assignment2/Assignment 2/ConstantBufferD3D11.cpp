#include "ConstantBufferD3D11.h"

#include <stdexcept>

ConstantBufferD3D11::ConstantBufferD3D11(ID3D11Device* device, UINT byteSize, void* initialData)
{
	Initialize(device, byteSize, initialData);
}

ConstantBufferD3D11::~ConstantBufferD3D11()
{
	if (buffer != nullptr)
		buffer->Release();
}

ConstantBufferD3D11::ConstantBufferD3D11(ConstantBufferD3D11&& other) noexcept :
	buffer(other.buffer), bufferSize(other.bufferSize)
{
	other.buffer = nullptr;
	other.bufferSize = 0;
}

ConstantBufferD3D11& ConstantBufferD3D11::operator=(ConstantBufferD3D11&& other) noexcept
{
	if (this != &other)
	{
		buffer = other.buffer;
		other.buffer = nullptr;
		bufferSize = other.bufferSize;
		other.bufferSize = 0;
	}

	return *this;
}

void ConstantBufferD3D11::Initialize(ID3D11Device* device, UINT byteSize, void* initialData)
{
	bufferSize = byteSize;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HRESULT hr = S_OK;

	if (initialData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = initialData;
		data.SysMemPitch = data.SysMemSlicePitch = 0;

		hr = device->CreateBuffer(&desc, &data, &buffer);
	}
	else
	{
		hr = device->CreateBuffer(&desc, nullptr, &buffer);
	}

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create constant buffer");
	}
}

UINT ConstantBufferD3D11::GetSize() const
{
	return bufferSize;
}

ID3D11Buffer* ConstantBufferD3D11::GetBuffer() const
{
	return buffer;
}

void ConstantBufferD3D11::UpdateBuffer(ID3D11DeviceContext* context, void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);

	memcpy(mappedBuffer.pData, data, bufferSize);

	context->Unmap(buffer, 0);
}
