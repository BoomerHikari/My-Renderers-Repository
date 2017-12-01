#include "Graphics_DX11.h"

//Macro for releasing DirectX Com Pointers
#define SafeRelease(x) if (x != nullptr) {x->Release(); x = nullptr;}

//DXGI includes
#include <dxgi1_2.h>
#pragma comment(lib, "Dxgi.lib")

#pragma once

Graphics_DX11::Graphics_DX11() : device(nullptr), immediateContext(nullptr), primaryRenderTargetTexture(nullptr), primaryTextureRTV(nullptr)
{
	ZeroMemory(&fullScreenViewport, sizeof(D3D11_VIEWPORT));
	supportedFeatureLevel = D3D_FEATURE_LEVEL_10_0;
}

Graphics_DX11::~Graphics_DX11()
{
	Release();
}

void Graphics_DX11::Release()
{
	SafeRelease(primaryTextureRTV);
	SafeRelease(primaryRenderTargetTexture);
	SafeRelease(swapChain);
	SafeRelease(immediateContext);
	SafeRelease(device);
}

void Graphics_DX11::Init(HWND hwnd)
{
	D3D_FEATURE_LEVEL PossibleFeatureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	HRESULT result;

#if _DEBUG
	result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, PossibleFeatureLevels, 4, D3D11_SDK_VERSION, &device, &supportedFeatureLevel, &immediateContext);
#else
	result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, PossibleFeatureLevels, 4, D3D11_SDK_VERSION, &device, &SupportedFeatureLevel, &immediateContext);
#endif

	//Long-Term goal is to not use an hwnd, and simply return the rendered texture.
	//Need this for now, though.
	if (hwnd != nullptr)
	{
		//This factory will be used to build the swap chain.
		IDXGIFactory* factory;
		result = CreateDXGIFactory(__uuidof(factory), (void**)(&factory));

		if (result == S_OK)
		{
			//Building the swap chain description
			DXGI_SWAP_CHAIN_DESC SCdesc;
			ZeroMemory(&SCdesc, sizeof(DXGI_SWAP_CHAIN_DESC));
			SCdesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
			SCdesc.OutputWindow = hwnd;
			SCdesc.BufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			SCdesc.BufferDesc.RefreshRate.Numerator = 1;
			SCdesc.BufferDesc.RefreshRate.Denominator = 60;
			SCdesc.BufferCount = 1;
			SCdesc.Windowed = true;

			//Create the swap chain with the factory we made earlier
			factory->CreateSwapChain(device, &SCdesc, &swapChain);
		}
		//Release the IDXGIFactory if it exists.
		SafeRelease(factory);

		if (swapChain != nullptr)
		{
			D3D11_RENDER_TARGET_VIEW_DESC RTVdesc;
			ZeroMemory(&RTVdesc, sizeof(RTVdesc));
			RTVdesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			RTVdesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;

			swapChain->GetBuffer(0, __uuidof(primaryRenderTargetTexture), (void**)(&primaryRenderTargetTexture));
			device->CreateRenderTargetView(primaryRenderTargetTexture, &RTVdesc, &primaryTextureRTV);

			DXGI_SWAP_CHAIN_DESC SCdesc;
			swapChain->GetDesc(&SCdesc);

			fullScreenViewport.Height = SCdesc.BufferDesc.Height;
			fullScreenViewport.Width = SCdesc.BufferDesc.Width;
			fullScreenViewport.MaxDepth = 1;
		}
	}
}
