#pragma once

#include <d3d11_4.h>
#pragma comment (lib, "d3d11.lib")

class Graphics_DX11
{

	ID3D11Device*			device;
	ID3D11DeviceContext*	immediateContext;
	D3D11_VIEWPORT			fullScreenViewport;

	D3D_FEATURE_LEVEL		supportedFeatureLevel;
	
	IDXGISwapChain* swapChain;
	//TEXTURES

	//Rendered-to and final output texture
	ID3D11Texture2D*		primaryRenderTargetTexture;
	ID3D11RenderTargetView* primaryTextureRTV;




	Graphics_DX11();
	~Graphics_DX11();


public:
	void Release();
	void Init(HWND hwnd = nullptr);

};