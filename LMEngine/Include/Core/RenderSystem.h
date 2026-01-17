#pragma once
#include <Prerequisites.h>

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

public:
	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContextPtr getDeviceContext();
	VertexBufferPtr createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list);
	ConstantBufferPtr createConstantBuffer(void* buffer, UINT size_buffer);
	IndexBufferPtr createIndexBuffer(void* list_indices, UINT size_list);
	VertexShaderPtr createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShaderPtr createPixelShader(const void* shader_byte_code, size_t byte_code_size);

public:
	bool compileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();
	void setRasterizerState(bool cull_front);

private:
	void compilePrivateShaders();
	void initRasterizerState();

private:
	DeviceContextPtr m_deviceContext;

private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_immediateDeviceContext;

	Microsoft::WRL::ComPtr<ID3D11Device> m_d3d_device;
	Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory;

	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cull_front_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cull_back_state;

	Microsoft::WRL::ComPtr<ID3DBlob> m_vsblob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_psblob;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;

	unsigned char m_meshLayoutByteCode[1024];
	size_t m_meshLayoutSize = 0;

private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class ConstantBuffer;
	friend class Texture;
};


