#pragma once

#include <d3d11_4.h>

class ConstantBufferD3D11
{
private:
	ID3D11Buffer* buffer = nullptr;
	UINT bufferSize = 0;

public:
	ConstantBufferD3D11() = default;
	ConstantBufferD3D11(ID3D11Device* device, UINT byteSize, void* initialData = nullptr);
	~ConstantBufferD3D11();
	ConstantBufferD3D11(const ConstantBufferD3D11& other) = delete;
	ConstantBufferD3D11& operator=(const ConstantBufferD3D11& other) = delete;
	ConstantBufferD3D11(ConstantBufferD3D11&& other) noexcept; // Move constructor
	ConstantBufferD3D11& operator=(ConstantBufferD3D11&& other) noexcept; // Move assignment operator

	void Initialize(ID3D11Device* device, UINT byteSize, void* initialData = nullptr);

	UINT GetSize() const;
	ID3D11Buffer* GetBuffer() const;

	void UpdateBuffer(ID3D11DeviceContext* context, void* data);
};