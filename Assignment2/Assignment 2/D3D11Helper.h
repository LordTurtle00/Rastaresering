#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <iostream>

#include "D3DPtr.h"

struct GpuInterfaces
{
	D3DPtr<ID3D11Device> device;
	D3DPtr<ID3D11DeviceContext> immediateContext;
};

struct SwapChain
{
	D3DPtr<IDXGISwapChain> gpuInterface;
	D3DPtr<ID3D11Texture2D> backbuffer;
	D3DPtr<ID3D11RenderTargetView> rtv;
};

GpuInterfaces CreateGpuInterfaces();

SwapChain CreateSwapChain(ID3D11Device* device, UINT width, UINT height,
	HWND window);

bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device *& device, ID3D11DeviceContext *& immediateContext, IDXGISwapChain *& swapChain, ID3D11RenderTargetView *& rtv, D3D11_VIEWPORT & viewport);
