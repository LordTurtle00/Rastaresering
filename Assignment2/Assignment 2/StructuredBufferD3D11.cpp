#include "StructuredBufferD3D11.h"

#include <stdexcept>

StructuredBufferD3D11::StructuredBufferD3D11(ID3D11Device* device, UINT sizeOfElement,
	UINT nrOfElementsInBuffer, void* bufferData, bool dynamic)
{
	Initialize(device, sizeOfElement, nrOfElementsInBuffer, bufferData, dynamic);
}

StructuredBufferD3D11::~StructuredBufferD3D11()
{
	if (buffer != nullptr)
		buffer->Release();

	if (srv != nullptr)
		srv->Release();

	if (uav != nullptr)
		uav->Release();
}

void StructuredBufferD3D11::Initialize(ID3D11Device* device, UINT sizeOfElement,
	UINT nrOfElementsInBuffer, void* bufferData, bool dynamic, bool hasSRV, bool hasUAV)
{
	elementSize = sizeOfElement;
	nrOfElements = nrOfElementsInBuffer;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = elementSize * nrOfElements;
	desc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : hasUAV ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = hasSRV ? D3D11_BIND_SHADER_RESOURCE : 0;
	desc.BindFlags |= hasUAV ? D3D11_BIND_UNORDERED_ACCESS : 0;
	desc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;

	HRESULT hr = S_OK;

	if (bufferData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = bufferData;
		data.SysMemPitch = data.SysMemSlicePitch = 0;

		hr = device->CreateBuffer(&desc, &data, &buffer);
	}
	else
	{
		hr = device->CreateBuffer(&desc, nullptr, &buffer);
	}

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create structured buffer");
	}

	if (hasSRV)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = nrOfElements;

		hr = device->CreateShaderResourceView(buffer, &srvDesc, &srv);

		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create srv for structured buffer");
		}
	}

	if (hasUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = nrOfElements;
		uavDesc.Buffer.Flags = 0;

		hr = device->CreateUnorderedAccessView(buffer, &uavDesc, &uav);

		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create uav for structured buffer");
		}
	}
}

void StructuredBufferD3D11::UpdateBuffer(ID3D11DeviceContext* context, void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);

	memcpy(mappedBuffer.pData, data, elementSize * nrOfElements);

	context->Unmap(buffer, 0);
}

UINT StructuredBufferD3D11::GetElementSize() const
{
	return elementSize;
}

size_t StructuredBufferD3D11::GetNrOfElements() const
{
	return nrOfElements;
}

ID3D11ShaderResourceView* StructuredBufferD3D11::GetSRV() const
{
	return srv;
}

ID3D11UnorderedAccessView* StructuredBufferD3D11::GetUAV() const
{
	return uav;
}
