#pragma once

#include <cstdint>

#include <d3d11_4.h>

class IndexBufferD3D11
{
private:
	ID3D11Buffer* buffer = nullptr;
	UINT nrOfIndices = 0;

public:
	IndexBufferD3D11() = default;
	IndexBufferD3D11(ID3D11Device* device, UINT nrOfIndicesInBuffer, uint32_t* indexData);
	~IndexBufferD3D11();
	IndexBufferD3D11(const IndexBufferD3D11& other) = delete;
	IndexBufferD3D11& operator=(const IndexBufferD3D11& other) = delete;
	IndexBufferD3D11(IndexBufferD3D11&& other) noexcept;
	IndexBufferD3D11& operator=(IndexBufferD3D11&& other) noexcept;

	void Initialize(ID3D11Device* device, UINT nrOfIndicesInBuffer, uint32_t* indexData);

	UINT GetNrOfIndices() const;
	ID3D11Buffer* GetBuffer() const;
};