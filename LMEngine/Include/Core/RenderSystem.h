#pragma once
#include "Global.h"
#include "Prerequisites.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

public:
	SwapChainPtr CreateSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContextPtr GetDeviceContext();
	VertexBufferPtr CreateVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);
	ConstantBufferPtr CreateConstantBuffer(void* buffer, UINT size_buffer);
	IndexBufferPtr CreateIndexBuffer(void* list_indices, UINT size_list);
	VertexShaderPtr CreateVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShaderPtr CreatePixelShader(const void* shader_byte_code, size_t byte_code_size);

public:
	bool CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size);
	bool CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size);
	void ReleaseCompiledShader();

private:
	DeviceContextPtr m_deviceContext;

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
	friend class Texture;
};


