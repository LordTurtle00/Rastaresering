#pragma once

#include <d3d11_4.h>

class StructuredBufferD3D11
{
private:
	ID3D11Buffer* buffer = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11UnorderedAccessView* uav = nullptr;
	UINT elementSize = 0;
	UINT nrOfElements = 0;

public:
	StructuredBufferD3D11() = default;
	StructuredBufferD3D11(ID3D11Device* device, UINT sizeOfElement,
		UINT nrOfElementsInBuffer, void* bufferData = nullptr, bool dynamic = true);
	~StructuredBufferD3D11();
	StructuredBufferD3D11(const StructuredBufferD3D11& other) = delete;
	StructuredBufferD3D11& operator=(const StructuredBufferD3D11& other) = delete;
	StructuredBufferD3D11(StructuredBufferD3D11&& other) = delete;
	StructuredBufferD3D11 operator=(StructuredBufferD3D11&& other) = delete;

	void Initialize(ID3D11Device* device, UINT sizeOfElement,
		UINT nrOfElementsInBuffer, void* bufferData = nullptr, bool dynamic = true,
		bool hasSRV = true, bool hasUAV = false);

	void UpdateBuffer(ID3D11DeviceContext* context, void* data);

	UINT GetElementSize() const;
	size_t GetNrOfElements() const;
	ID3D11ShaderResourceView* GetSRV() const;
	ID3D11UnorderedAccessView* GetUAV() const;
};