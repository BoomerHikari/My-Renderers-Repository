#include "Graphics_DX11.h"
#include <fstream>
#include <vector>

//DXGI includes
#include <dxgi1_2.h>
#pragma comment(lib, "Dxgi.lib")
//D3Dcompiler include for shader reflection
#include <D3Dcompiler.h>
#pragma comment(lib, "D3Dcompiler.lib")
#pragma once



using Microsoft::WRL::ComPtr;

#pragma region PipelineState

class PipelineState
{
	//Buffers & Textures
	ID3D11RenderTargetView*		currentRTV;
	ID3D11DepthStencilView*		currentDSV;
	ID3D11Buffer*				currentVertexBuffer;
	ID3D11Buffer*				currentIndexBuffer;
	std::vector<ID3D11Buffer*>	currentVertexShaderConstantBuffers;
	std::vector<ID3D11Buffer*>	currentHullShaderConstantBuffers;
	std::vector<ID3D11Buffer*>	currentDomainShaderConstantBuffers;
	std::vector<ID3D11Buffer*>	currentGeometryShaderConstantBuffers;
	std::vector<ID3D11Buffer*>	currentPixelShaderConstantBuffers;
	//Shaders				 
	ID3D11VertexShader*			currentVertexShader;
	ID3D11HullShader*			currentHullShader;
	ID3D11DomainShader*			currentDomainShader;
	ID3D11GeometryShader*		currentGeometryShader;
	ID3D11PixelShader*			currentPixelShader;
	//Other						
	ID3D11DeviceContext*		immediateContextReference;
	D3D11_VIEWPORT*				currentViewport;
	UINT						currentVertexBufferOffset;
	//ID3D11DepthStencilState*	currentDepthStencilState;

public:

	PipelineState();

	//Shader Setters
	void SetVertexShader(ID3D11VertexShader* shader);
	void SetHullShader(ID3D11HullShader* shader);
	void SetDomainShader(ID3D11DomainShader* shader);
	void SetGeometryShader(ID3D11GeometryShader* shader);
	void SetPixelShader(ID3D11PixelShader* shader);

	//Constant Buffer Setters
	//buffers is empty when fucntion returns
	void SetVSConstantBuffers(std::vector<ID3D11Buffer*>& buffers);
	void SetHSConstantBuffers(std::vector<ID3D11Buffer*>& buffers);
	void SetDSConstantBuffers(std::vector<ID3D11Buffer*>& buffers);
	void SetGSConstantBuffers(std::vector<ID3D11Buffer*>& buffers);
	void SetPSConstantBuffers(std::vector<ID3D11Buffer*>& buffers);

	//Shader Resource Setters
	//TODO: Store & Set ShaderResourceViews

	//Other Setters
	void SetVertexIndexBuffers(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT strideLength, UINT offsetDistance = 0);
	void SetViewport(D3D11_VIEWPORT* viewport);
	void SetImmediateContext(ID3D11DeviceContext* context);
	void SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
	//void SetDepthStencilState(ID3D11DepthStencilState* DSstate);

};

//Constructors
PipelineState::PipelineState()
{
	ZeroMemory(this, sizeof(PipelineState));
}

//Shader Setters
void PipelineState::SetVertexShader(ID3D11VertexShader* shader)
{
	if (shader != currentVertexShader)
	{
		currentVertexShader = shader;

		immediateContextReference->VSSetShader(shader, NULL, 0);
	}
}

void PipelineState::SetHullShader(ID3D11HullShader* shader)
{
	if (currentHullShader != shader)
	{
		currentHullShader = shader;

		immediateContextReference->HSSetShader(shader, NULL, 0);
	}
}

void PipelineState::SetDomainShader(ID3D11DomainShader* shader)
{
	if (currentDomainShader != shader)
	{
		currentDomainShader = shader;
		
		immediateContextReference->DSSetShader(shader, NULL, 0);
	}
}

void PipelineState::SetGeometryShader(ID3D11GeometryShader* shader)
{
	if (currentGeometryShader != shader)
	{
		currentGeometryShader = shader;
		immediateContextReference->GSSetShader(shader, NULL, 0);
	}
}

void PipelineState::SetPixelShader(ID3D11PixelShader* shader)
{
	if (currentPixelShader != shader)
	{
		currentPixelShader = shader;
		
		immediateContextReference->PSSetShader(shader, NULL, 0);
	}
}

//Constant Buffer Setters

void PipelineState::SetVSConstantBuffers(std::vector<ID3D11Buffer*>& buffers) 
{
	if (currentVertexShaderConstantBuffers != buffers)
	{
		currentVertexShaderConstantBuffers.swap(buffers);
		buffers.clear();

		immediateContextReference->VSSetConstantBuffers(0, buffers.size(), currentVertexShaderConstantBuffers.data());
	}
}

void PipelineState::SetHSConstantBuffers(std::vector<ID3D11Buffer*>& buffers) 
{
	if (currentHullShaderConstantBuffers != buffers)
	{
		currentHullShaderConstantBuffers.swap(buffers);
		buffers.clear();

		immediateContextReference->HSSetConstantBuffers(0, buffers.size(), currentHullShaderConstantBuffers.data());
	}
}

void PipelineState::SetDSConstantBuffers(std::vector<ID3D11Buffer*>& buffers) 
{
	if (currentDomainShaderConstantBuffers != buffers)
	{
		currentDomainShaderConstantBuffers.swap(buffers);
		buffers.clear();

		immediateContextReference->DSSetConstantBuffers(0, buffers.size(), currentDomainShaderConstantBuffers.data());
	}
}

void PipelineState::SetGSConstantBuffers(std::vector<ID3D11Buffer*>& buffers) 
{
	if (currentGeometryShaderConstantBuffers != buffers)
	{
		currentGeometryShaderConstantBuffers.swap(buffers);
		buffers.clear();

		immediateContextReference->GSSetConstantBuffers(0, buffers.size(), currentGeometryShaderConstantBuffers.data());
	}
}

void PipelineState::SetPSConstantBuffers(std::vector<ID3D11Buffer*>& buffers) 
{
	if (currentPixelShaderConstantBuffers != buffers)
	{
		currentPixelShaderConstantBuffers.swap(buffers);
		buffers.clear();

		immediateContextReference->PSSetConstantBuffers(0, buffers.size(), currentPixelShaderConstantBuffers.data());
	}
}

//Other Setters
void PipelineState::SetImmediateContext(ID3D11DeviceContext* context)
{
	immediateContextReference = context;
}

void PipelineState::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	if (currentRTV != rtv || currentDSV != dsv)
	{
		currentRTV = rtv;
		currentDSV = dsv;

		immediateContextReference->OMSetRenderTargets(1, &rtv, dsv);
	}
}

/*void PipelineState::SetDepthStencilState(ID3D11DepthStencilState* DSstate)
{
	if (currentDepthStencilState != DSstate)
	{
		currentDepthStencilState = DSstate;
		immediateContextReference->OMGetDepthStencilState(&DSstate, UINT(0));
	}
}*/

void PipelineState::SetVertexIndexBuffers(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT vertexSize, UINT offsetDistance)
{
	//This will need to change for 
	if (vertexBuffer != currentVertexBuffer || offsetDistance != currentVertexBufferOffset)
	{
		immediateContextReference->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offsetDistance);
	}
	if (indexBuffer != currentIndexBuffer)
	{
		immediateContextReference->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}
}

void PipelineState::SetViewport(D3D11_VIEWPORT* viewport)
{
	if (currentViewport != viewport)
	{
		currentViewport = viewport;

		immediateContextReference->RSSetViewports(1, viewport);
	}
}

#pragma endregion



Graphics_DX11::Graphics_DX11() : device(nullptr), immediateContext(nullptr), primaryRenderTargetTexture(nullptr), primaryTextureRTV(nullptr)
{
	ZeroMemory(&fullScreenViewport, sizeof(D3D11_VIEWPORT));
	ZeroMemory(&currentPipeline, sizeof(currentPipeline));
	supportedFeatureLevel = D3D_FEATURE_LEVEL_10_0;
}

Graphics_DX11::~Graphics_DX11()
{
	Release();
}

unsigned int Graphics_DX11::GetFileBinary(char * file, char ** fileData)
{
	unsigned int Flen = 0;
	std::ifstream reader;
	reader.open(file, std::ios_base::binary);

	if (reader.is_open())
	{
		//Read the binary file into a char array
		reader.seekg(0, reader.end);
		Flen = reader.tellg();
		if (Flen > 0)
		{
			*fileData = new char[Flen];
			reader.seekg(0, reader.beg);
			reader.read(*fileData, Flen);
		}
		reader.close();
	}
	
	return Flen;
}

unsigned int Graphics_DX11::LoadVertexShader(char * file)
{
	unsigned int VSindex = 0;
	char* VSbytecode = nullptr;

	//Read the shader file into a char array
	unsigned int BClen = GetFileBinary(file, &VSbytecode);

	if (BClen > 0)
	{
		//Our ultimate goals are to load these
		ComPtr<ID3D11InputLayout> inputLayout = nullptr;
		ComPtr<ID3D11VertexShader> vertexShader = nullptr;

		//Start building the input layout with shader reflection
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> VSreflection = nullptr;
		D3D11_SHADER_DESC VSdesc;
		//Reflect the shader
		D3D11_SIGNATURE_PARAMETER_DESC SPdesc;
		D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc = nullptr;
		if (D3DReflect(VSbytecode, BClen, IID_ID3D11ShaderReflection, (void**)(&VSreflection)) == S_OK);
		{
			//Get the shader's description
			VSreflection->GetDesc(&VSdesc);

			//It was about this time that I realised this was nearly pointless, but I had already started.

			if (VSdesc.InputParameters > 0)
			{
				inputLayoutDesc = new D3D11_INPUT_ELEMENT_DESC[VSdesc.InputParameters];
			}
			for (UINT i = 0; i < VSdesc.InputParameters; ++i)
			{
				D3D11_SHADER_INPUT_BIND_DESC SIBdesc;
				VSreflection->GetResourceBindingDesc(i, &SIBdesc);
				VSreflection->GetInputParameterDesc(i, &SPdesc);
				inputLayoutDesc[i].SemanticName = SPdesc.SemanticName;
				inputLayoutDesc[i].SemanticIndex = SPdesc.SemanticIndex;
				inputLayoutDesc[i].InputSlot = 0;
				inputLayoutDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				inputLayoutDesc[i].InstanceDataStepRate = 0;
				inputLayoutDesc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

				DXGI_FORMAT ILformat;

				switch (SPdesc.ComponentType)
				{
					case (D3D_REGISTER_COMPONENT_FLOAT32): {
						if (SPdesc.Mask == 1)
						{
							ILformat = DXGI_FORMAT_R32_FLOAT;
						}
						else if (SPdesc.Mask < 4)
						{
							ILformat = DXGI_FORMAT_R32G32_FLOAT;
						}
						else if (SPdesc.Mask < 8)
						{
							ILformat = DXGI_FORMAT_R32G32B32_FLOAT;
						}
						else
						{
							ILformat = DXGI_FORMAT_R32G32B32A32_FLOAT;
						}
						break;
					}
					case (D3D_REGISTER_COMPONENT_SINT32): {
						if (SPdesc.Mask == 1)
						{
							ILformat = DXGI_FORMAT_R32_SINT;
						}
						else if (SPdesc.Mask < 4)
						{
							ILformat = DXGI_FORMAT_R32G32_SINT;
						}
						else if (SPdesc.Mask < 8)
						{
							ILformat = DXGI_FORMAT_R32G32B32_SINT;
						}
						else
						{
							ILformat = DXGI_FORMAT_R32G32B32A32_SINT;
						}
						break;
					}
					case (D3D_REGISTER_COMPONENT_UINT32): {
						if (SPdesc.Mask == 1)
						{
							ILformat = DXGI_FORMAT_R32_UINT;
						}
						else if (SPdesc.Mask < 4)
						{
							ILformat = DXGI_FORMAT_R32G32_UINT;
						}
						else if (SPdesc.Mask < 8)
						{
							ILformat = DXGI_FORMAT_R32G32B32_UINT;
						}
						else
						{
							ILformat = DXGI_FORMAT_R32G32B32A32_UINT;
						}
						break;
				}
					default: {
						ILformat = DXGI_FORMAT_R32G32B32A32_FLOAT;
					}
			}
				inputLayoutDesc[i].Format = ILformat;
		}

			
	}
		if (inputLayoutDesc != nullptr)
		{
			device->CreateInputLayout(inputLayoutDesc, VSdesc.InputParameters, VSbytecode, BClen, &inputLayout);
			device->CreateVertexShader(VSbytecode, BClen, NULL, &vertexShader);

			//These need to be stored somewhere.
		}


		delete[] inputLayoutDesc;
}

	delete VSbytecode;
	return VSindex;
}

void Graphics_DX11::Release()
{
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
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		result = CreateDXGIFactory(__uuidof(factory.Get()), (void**)(&factory));

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
			factory->CreateSwapChain(device.Get(), &SCdesc, &swapChain);
		}

		if (swapChain != nullptr)
		{
			DXGI_SWAP_CHAIN_DESC SCdesc;
			swapChain->GetDesc(&SCdesc);

			D3D11_RENDER_TARGET_VIEW_DESC RTVdesc;
			ZeroMemory(&RTVdesc, sizeof(RTVdesc));
			RTVdesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			RTVdesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;

			D3D11_TEXTURE2D_DESC T2Ddesc;
			ZeroMemory(&T2Ddesc, sizeof(T2Ddesc));
			T2Ddesc.Width = SCdesc.BufferDesc.Width;
			T2Ddesc.Height = SCdesc.BufferDesc.Height;
			T2Ddesc.MipLevels = 1;
			T2Ddesc.ArraySize = 1;
			T2Ddesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			T2Ddesc.SampleDesc.Count = 1;
			T2Ddesc.Usage = D3D11_USAGE_DEFAULT;
			T2Ddesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			swapChain->GetBuffer(0, __uuidof(primaryRenderTargetTexture.Get()), (void**)(primaryRenderTargetTexture.ReleaseAndGetAddressOf()));
			device->CreateRenderTargetView(primaryRenderTargetTexture.Get(), &RTVdesc, primaryTextureRTV.ReleaseAndGetAddressOf());

			device->CreateTexture2D(&T2Ddesc, NULL, primaryDepthBufferTexture.ReleaseAndGetAddressOf());
			device->CreateDepthStencilView(primaryDepthBufferTexture.Get(), NULL, primaryDepthBuffer.ReleaseAndGetAddressOf());

			fullScreenViewport.Height = SCdesc.BufferDesc.Height;
			fullScreenViewport.Width = SCdesc.BufferDesc.Width;
			fullScreenViewport.MaxDepth = 1;
		}
	}
}