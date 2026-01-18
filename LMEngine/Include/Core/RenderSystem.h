#pragma once
#include <Prerequisites.h>
#include <Texture2D.h>

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

	VertexShaderPtr createVertexShader(const wchar_t* full_path, const char* entryPoint);
	PixelShaderPtr createPixelShader(const wchar_t* full_path, const char* entryPoint);

	Texture2DPtr createTexture(const wchar_t* full_path);
	Texture2DPtr createTexture(const Rect& size, Texture2D::Type type);

public:
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
	friend class Texture2D;
};


