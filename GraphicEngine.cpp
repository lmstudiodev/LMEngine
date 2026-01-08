#include "GraphicEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "d3dcompiler.h"
#include "InputSystem.h"

GraphicEngine::GraphicEngine() : m_d3d_device(nullptr),
m_deviceContext(nullptr),
m_dxgiDevice(nullptr), 
m_dxgiAdapter(nullptr), 
m_dxgiFactory(nullptr),
m_blob(nullptr)
{
}

GraphicEngine::~GraphicEngine()
{
}

bool GraphicEngine::Init()
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

	m_deviceContext = new DeviceContext(m_immediateDeviceContext);

	if (FAILED(m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice)))
		return false;

	if (FAILED(m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter)))
		return false;

	if (FAILED(m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory)))
		return false;

	return true;
}

bool GraphicEngine::Release()
{
	m_dxgiDevice->Release();
	m_dxgiAdapter->Release();
	m_dxgiFactory-> Release();
	m_deviceContext->Release();
	m_d3d_device->Release();

	return true;
}

SwapChain* GraphicEngine::CreateSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicEngine::GetDeviceContext()
{
	return this->m_deviceContext;
}

VertexBuffer* GraphicEngine::CreateVertexBuffer()
{
	return new VertexBuffer();
}

ConstantBuffer* GraphicEngine::CreateConstantBuffer()
{
	return new ConstantBuffer();
}

IndexBuffer* GraphicEngine::CreateIndexBuffer()
{
	return new IndexBuffer();
}

VertexShader* GraphicEngine::CreateVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = new VertexShader();

	if (!vs->Init(shader_byte_code, byte_code_size))
	{
		vs->Release();
		return nullptr;
	}

	return vs;
}

PixelShader* GraphicEngine::CreatePixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = new PixelShader();

	if (!ps->Init(shader_byte_code, byte_code_size))
	{
		ps->Release();
		return nullptr;
	}

	return ps;
}

GraphicEngine* GraphicEngine::Get()
{
	static GraphicEngine engine;
	
	return &engine;
}

bool GraphicEngine::CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size)
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

bool GraphicEngine::CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size)
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

void GraphicEngine::ReleaseCompiledShader()
{
	if(this->m_blob)
		this->m_blob->Release();
}