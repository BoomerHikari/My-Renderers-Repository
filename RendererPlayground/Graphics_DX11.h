#pragma once

//Smart Pointer header
#include <wrl.h>

#include <d3d11_4.h>
#pragma comment (lib, "d3d11.lib")

class PipelineState;

class Graphics_DX11
{
	enum class ShaderType
	{
		Vertex,
		Hull,
		Domain,
		Geography,
		Pixel,
		Compute
	};

	PipelineState* currentPipeline;

	Microsoft::WRL::ComPtr<ID3D11Device>	device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	immediateContext;
	D3D11_VIEWPORT			fullSurfaceViewport;

	D3D_FEATURE_LEVEL		supportedFeatureLevel;
	
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	//TEXTURES
	//Rendered-to and final output texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			primaryRenderTargetTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	primaryTextureRTV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			primaryDepthBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	primaryDepthBuffer;
	


	Graphics_DX11();
	~Graphics_DX11();

	//Helper Functions
	unsigned int GetFileBinary(char* file, char** fileData);

	//Functional Functions
	unsigned int LoadVertexShader(char* file);
	unsigned int LoadPixelShader(char* file);
	unsigned int LoadHullShader(char* file);
	unsigned int LoadDomainShader(char* file);
	unsigned int LoadGeometryShader(char* file);

public:
	void Release();
	void Init(HWND hwnd = nullptr);
	unsigned int LoadShader(char* file, ShaderType type);

};