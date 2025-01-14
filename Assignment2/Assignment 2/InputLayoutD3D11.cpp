#include "InputLayoutD3D11.h"

#include <stdexcept>

InputLayoutD3D11::~InputLayoutD3D11()
{
	if (inputLayout != nullptr)
		inputLayout->Release();
}

void InputLayoutD3D11::AddInputElement(const std::string& semanticName, DXGI_FORMAT format,
	UINT semanticIndex)
{
	semanticNames.push_back(semanticName);
	elements.push_back(D3D11_INPUT_ELEMENT_DESC());
	elements.back().SemanticIndex = semanticIndex;
	elements.back().Format = format;
	elements.back().InputSlot = 0;
	elements.back().AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elements.back().InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elements.back().InstanceDataStepRate = 0;
}

void InputLayoutD3D11::FinalizeInputLayout(ID3D11Device* device, const void* vsDataPtr, size_t vsDataSize)
{
	for (size_t i = 0; i < semanticNames.size(); ++i)
	{
		elements[i].SemanticName = semanticNames[i].c_str();
	}

	HRESULT hr = device->CreateInputLayout(elements.data(), static_cast<UINT>(elements.size()), 
		vsDataPtr, vsDataSize, &inputLayout);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create input layout");
	}
}

ID3D11InputLayout* InputLayoutD3D11::GetInputLayout() const
{
	return inputLayout;
}
