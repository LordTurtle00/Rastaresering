#include "IndexBufferD3D11.h"

#include <stdexcept>

IndexBufferD3D11::IndexBufferD3D11(ID3D11Device* device, UINT nrOfIndicesInBuffer, uint32_t* indexData)
{
	Initialize(device, nrOfIndicesInBuffer, indexData);
}

IndexBufferD3D11::~IndexBufferD3D11()
{
	if(buffer != nullptr)
		buffer->Release();
}

IndexBufferD3D11::IndexBufferD3D11(IndexBufferD3D11&& other) noexcept : 
	buffer(other.buffer), nrOfIndices(other.nrOfIndices)
{
	other.buffer = nullptr;
	other.nrOfIndices = 0;
}

IndexBufferD3D11& IndexBufferD3D11::operator=(IndexBufferD3D11&& other) noexcept
{
	if (this != &other)
	{
		buffer = other.buffer;
		nrOfIndices = other.nrOfIndices;

		other.buffer = nullptr;
		other.nrOfIndices = 0;
	}

	return *this;
}

void IndexBufferD3D11::Initialize(ID3D11Device* device, UINT nrOfIndicesInBuffer, uint32_t* indexData)
{
	nrOfIndices = nrOfIndicesInBuffer;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = nrOfIndicesInBuffer * sizeof(uint32_t);
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = indexData;
	data.SysMemPitch = data.SysMemSlicePitch = 0;

	HRESULT hr = S_OK;
	hr = device->CreateBuffer(&desc, &data, &buffer);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create constant buffer");
	}
}

UINT IndexBufferD3D11::GetNrOfIndices() const
{
	return nrOfIndices;
}

ID3D11Buffer* IndexBufferD3D11::GetBuffer() const
{
	return buffer;
}
