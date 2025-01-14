#include "D3D11Helper.h"

#include <exception>

void ThrowIfFailed(HRESULT hr, const std::exception& exception)
{
	if (FAILED(hr))
	{
		throw exception;
	}
}

GpuInterfaces CreateGpuInterfaces()
{
	GpuInterfaces toReturn;

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
		flags, featureLevels, 1, D3D11_SDK_VERSION, &toReturn.device, nullptr,
		&toReturn.immediateContext);

	ThrowIfFailed(hr, std::runtime_error("Could not create gpu interfaces"));

	return toReturn;
}

SwapChain CreateSwapChain(ID3D11Device* device, UINT width, UINT height, 
	HWND window)
{
	SwapChain toReturn;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	IDXGIFactory* factory = nullptr;
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	ThrowIfFailed(hr, std::runtime_error("Could not create dxgi factory"));
	hr = factory->CreateSwapChain(device, &swapChainDesc, &toReturn.gpuInterface);
	ThrowIfFailed(hr, std::runtime_error("Could not create dxgi swap chain"));
	factory->Release();

	hr = toReturn.gpuInterface->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&toReturn.backbuffer));
	ThrowIfFailed(hr, std::runtime_error("Could not fetch backbuffer from swap chain"));
	hr = device->CreateRenderTargetView(toReturn.backbuffer, nullptr, &toReturn.rtv);
	ThrowIfFailed(hr, std::runtime_error("Could not create rtv for backbuffer"));

	return toReturn;
}

bool CreateInterfaces(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, UINT width, UINT height, HWND window)
{
	UINT flags = 0;
#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Default
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	IDXGIFactory* factory;
	CreateDXGIFactory(IID_PPV_ARGS(&factory));
	UINT adapterIndex = 0;
	UINT bestIndex = 0;
	size_t bestMemory = 0;
	IDXGIAdapter* adapter = nullptr;
	factory->EnumAdapters(adapterIndex, &adapter);
	do
	{
		DXGI_ADAPTER_DESC adapterDesc;
		adapter->GetDesc(&adapterDesc);

		if (adapterDesc.DedicatedVideoMemory > bestMemory)
		{
			bestMemory = adapterDesc.DedicatedVideoMemory;
			bestIndex = adapterIndex;
		}

		adapter->Release();
		factory->EnumAdapters(++adapterIndex, &adapter);

	} while (adapter != nullptr);

	factory->EnumAdapters(bestIndex, &adapter);
	HRESULT hr = D3D11CreateDeviceAndSwapChain(adapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &immediateContext);
	factory->Release();
	adapter->Release();

	return !(FAILED(hr));
}

bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv)
{
	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	HRESULT hr = device->CreateRenderTargetView(backBuffer, NULL, &rtv);
	backBuffer->Release();
	return !(FAILED(hr));

}

void SetViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device*& device,
	ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv,
	D3D11_VIEWPORT& viewport)
{
	if (!CreateInterfaces(device, immediateContext, swapChain, width, height, window))
	{
		std::cerr << "Error creating interfaces!" << std::endl;
		return false;
	}

	if (!CreateRenderTargetView(device, swapChain, rtv))
	{
		std::cerr << "Error creating rtv!" << std::endl;
		return false;
	}

	SetViewport(viewport, width, height);

	return true;
}