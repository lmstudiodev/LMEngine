#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

public:
	bool Init();
	bool Release();

public:
	SwapChain* CreateSwapChain();
	DeviceContext* GetDeviceContext();
	VertexBuffer* CreateVertexBuffer();
	ConstantBuffer* CreateConstantBuffer();
	IndexBuffer* CreateIndexBuffer();
	VertexShader* CreateVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShader* CreatePixelShader(const void* shader_byte_code, size_t byte_code_size);

public:
	bool CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size);
	bool CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size);
	void ReleaseCompiledShader();

private:
	DeviceContext* m_deviceContext;

private:
	ID3D11Device* m_d3d_device;

private:
	IDXGIDevice* m_dxgiDevice;
	IDXGIAdapter* m_dxgiAdapter;
	IDXGIFactory* m_dxgiFactory;
	ID3DBlob* m_blob;

private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class ConstantBuffer;
};


