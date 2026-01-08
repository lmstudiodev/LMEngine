#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "d3dcompiler.h"
#include "InputSystem.h"
#include <exception>

RenderSystem::RenderSystem() : m_d3d_device(nullptr),
m_deviceContext(nullptr),
m_dxgiDevice(nullptr),
m_dxgiAdapter(nullptr),
m_dxgiFactory(nullptr),
m_blob(nullptr)
{
}

RenderSystem::~RenderSystem()
{
}

bool RenderSystem::Init()
{
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT driverTypesCount = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	UINT featureLevelsCount = ARRAYSIZE(featureLevels);

	HRESULT hr = 0;

	ID3D11DeviceContext* m_immediateDeviceContext;
	D3D_FEATURE_LEVEL m_featureLevel;

	for (UINT dtIndex = 0; dtIndex < driverTypesCount;)
	{
		hr = D3D11CreateDevice(NULL, driverTypes[dtIndex], NULL, NULL, featureLevels, featureLevelsCount, D3D11_SDK_VERSION, &m_d3d_device, &m_featureLevel, &m_immediateDeviceContext);

		if (SUCCEEDED(hr))
			break;

		++dtIndex;
	}

	if (FAILED(hr))
		return false;

	m_deviceContext = new DeviceContext(m_immediateDeviceContext, this);

	if (FAILED(m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice)))
		return false;

	if (FAILED(m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter)))
		return false;

	if (FAILED(m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory)))
		return false;

	return true;
}

bool RenderSystem::Release()
{
	m_dxgiDevice->Release();
	m_dxgiAdapter->Release();
	m_dxgiFactory->Release();
	delete m_deviceContext;
	m_d3d_device->Release();

	return true;
}

SwapChain* RenderSystem::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
	return new SwapChain(hwnd, width, height, this);
}

DeviceContext* RenderSystem::GetDeviceContext()
{
	return this->m_deviceContext;
}

VertexBuffer* RenderSystem::CreateVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	return new VertexBuffer(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
}

ConstantBuffer* RenderSystem::CreateConstantBuffer(void* buffer, UINT size_buffer)
{
	return new ConstantBuffer(buffer, size_buffer, this);
}

IndexBuffer* RenderSystem::CreateIndexBuffer(void* list_indices, UINT size_list)
{
	return new IndexBuffer(list_indices, size_list, this);
}

VertexShader* RenderSystem::CreateVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = new VertexShader(shader_byte_code, byte_code_size, this);
	return vs;
}

PixelShader* RenderSystem::CreatePixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = new PixelShader(shader_byte_code, byte_code_size, this);
	return ps;
}

bool RenderSystem::CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* errblob = nullptr;

	if (FAILED(D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "vs_5_0", NULL, NULL, &m_blob, &errblob)))
	{
		if (errblob)
			errblob->Release();

		return false;
	}

	*shader_byte_code = this->m_blob->GetBufferPointer();
	*byte_code_size = (UINT)this->m_blob->GetBufferSize();

	return true;
}

bool RenderSystem::CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* errblob = nullptr;

	if (FAILED(D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "ps_5_0", NULL, NULL, &m_blob, &errblob)))
	{
		if (errblob)
			errblob->Release();

		return false;
	}

	*shader_byte_code = this->m_blob->GetBufferPointer();
	*byte_code_size = (UINT)this->m_blob->GetBufferSize();

	return true;
}

void RenderSystem::ReleaseCompiledShader()
{
	if (this->m_blob)
		this->m_blob->Release();
}