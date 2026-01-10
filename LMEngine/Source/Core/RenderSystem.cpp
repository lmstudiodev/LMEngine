#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputSystem.h"

RenderSystem::RenderSystem() : m_d3d_device(nullptr),
m_deviceContext(nullptr),
m_dxgiDevice(nullptr),
m_dxgiAdapter(nullptr),
m_dxgiFactory(nullptr),
m_blob(nullptr)
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
		throw std::exception("[D3D11 Error] D3D11CreateDevice creation failed.");

	m_deviceContext = std::make_shared<DeviceContext>(m_immediateDeviceContext, this);

	if (FAILED(m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice)))
		throw std::exception("[D3D11 Error] IDXGIDevice creation failed.");

	if (FAILED(m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter)))
		throw std::exception("[D3D11 Error] IDXGIAdapter creation failed.");

	if (FAILED(m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory)))
		throw std::exception("[D3D11 Error] IDXGIFactory creation failed.");
}

RenderSystem::~RenderSystem()
{
	m_dxgiDevice->Release();
	m_dxgiAdapter->Release();
	m_dxgiFactory->Release();
	m_d3d_device->Release();
}

SwapChainPtr RenderSystem::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChainPtr sc = nullptr;

	try
	{
		sc = std::make_shared<SwapChain>(hwnd, width, height, this);
	}
	catch (...) {}
	
	return sc;
}

DeviceContextPtr RenderSystem::GetDeviceContext()
{
	return this->m_deviceContext;
}

VertexBufferPtr RenderSystem::CreateVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	VertexBufferPtr vb = nullptr;

	try
	{
		vb = std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}
	catch(...){}
	
	return vb;
}

ConstantBufferPtr RenderSystem::CreateConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBufferPtr cb = nullptr;

	try
	{
		cb = std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
	}
	catch (...) {}
	
	return cb;
}

IndexBufferPtr RenderSystem::CreateIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBufferPtr ib = nullptr;

	try
	{
		ib = std::make_shared<IndexBuffer>(list_indices, size_list, this);
	}
	catch (...) {}
	
	return ib;
}

VertexShaderPtr RenderSystem::CreateVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShaderPtr vs = nullptr;

	try
	{
		vs = std::make_shared<VertexShader>(shader_byte_code, byte_code_size, this);
	}
	catch(...){}

	return vs;
}

PixelShaderPtr RenderSystem::CreatePixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShaderPtr ps = nullptr;

	try
	{
		ps = std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
	}
	catch(...){}

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