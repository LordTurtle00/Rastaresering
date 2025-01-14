#include "VertexBufferD3D11.h"

#include <stdexcept>

VertexBufferD3D11::VertexBufferD3D11(ID3D11Device* device, UINT sizeOfVertex,
	UINT nrOfVerticesInBuffer, void* vertexData) :
	vertexSize(vertexSize), nrOfVertices(nrOfVertices)
{
	Initialize(device, sizeOfVertex, nrOfVerticesInBuffer, vertexData);
}

VertexBufferD3D11::~VertexBufferD3D11()
{
	if (buffer != nullptr)
		buffer->Release();
}

VertexBufferD3D11::VertexBufferD3D11(VertexBufferD3D11&& other) : buffer(other.buffer),
	nrOfVertices(other.nrOfVertices), vertexSize(other.vertexSize)
{
	other.buffer = nullptr;
	other.nrOfVertices = 0;
	other.vertexSize = 0;
}

VertexBufferD3D11& VertexBufferD3D11::operator=(VertexBufferD3D11&& other)
{
	if (this != &other)
	{
		buffer = other.buffer;
		nrOfVertices = other.nrOfVertices;
		vertexSize = other.vertexSize;

		other.buffer = nullptr;
		other.nrOfVertices = 0;
		other.vertexSize = 0;
	}

	return *this;
}

void VertexBufferD3D11::Initialize(ID3D11Device* device, UINT sizeOfVertex,
	UINT nrOfVerticesInBuffer, void* vertexData)
{
	vertexSize = sizeOfVertex;
	nrOfVertices = nrOfVerticesInBuffer;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = vertexSize * nrOfVertices;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexData;
	data.SysMemPitch = data.SysMemSlicePitch = 0;

	HRESULT hr = S_OK;
	hr = device->CreateBuffer(&desc, &data, &buffer);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}
}

UINT VertexBufferD3D11::GetNrOfVertices() const
{
	return nrOfVertices;
}

UINT VertexBufferD3D11::GetVertexSize() const
{
	return vertexSize;
}

ID3D11Buffer* VertexBufferD3D11::GetBuffer() const
{
	return buffer;
}
