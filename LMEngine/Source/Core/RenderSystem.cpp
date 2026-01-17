#include <stdafx.h>
#include <RenderSystem.h>
#include <SwapChain.h>
#include <DeviceContext.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <ConstantBuffer.h>
#include <VertexShader.h>
#include <PixelShader.h>

RenderSystem::RenderSystem() : m_d3d_device(nullptr),
m_deviceContext(nullptr),
m_dxgiDevice(nullptr),
m_dxgiAdapter(nullptr),
m_dxgiFactory(nullptr),
m_blob(nullptr),
m_vsblob(nullptr),
m_psblob(nullptr),
m_vs(nullptr),
m_ps(nullptr),
m_cull_front_state(nullptr),
m_cull_back_state(nullptr)
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

	m_deviceContext = std::make_shared<DeviceContext>(m_immediateDeviceContext.Get(), this);

	if (FAILED(m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice)))
		throw std::exception("[D3D11 Error] IDXGIDevice creation failed.");

	if (FAILED(m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter)))
		throw std::exception("[D3D11 Error] IDXGIAdapter creation failed.");

	if (FAILED(m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory)))
		throw std::exception("[D3D11 Error] IDXGIFactory creation failed.");

	initRasterizerState();
	compilePrivateShaders();
}

RenderSystem::~RenderSystem()
{

}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChainPtr sc = nullptr;

	try
	{
		sc = std::make_shared<SwapChain>(hwnd, width, height, this);
	}
	catch (...) {}
	
	return sc;
}

DeviceContextPtr RenderSystem::getDeviceContext()
{
	return this->m_deviceContext;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list)
{
	return std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, this);;
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBufferPtr cb = nullptr;

	try
	{
		cb = std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
	}
	catch (...) {}
	
	return cb;
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBufferPtr ib = nullptr;

	try
	{
		ib = std::make_shared<IndexBuffer>(list_indices, size_list, this);
	}
	catch (...) {}
	
	return ib;
}

VertexShaderPtr RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShaderPtr vs = nullptr;

	try
	{
		vs = std::make_shared<VertexShader>(shader_byte_code, byte_code_size, this);
	}
	catch(...){}

	return vs;
}

PixelShaderPtr RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShaderPtr ps = nullptr;

	try
	{
		ps = std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
	}
	catch(...){}

	return ps;
}

Texture2DPtr RenderSystem::createTexture(const wchar_t* full_path)
{
	return std::make_shared<Texture2D>(full_path, this);
}

Texture2DPtr RenderSystem::createTexture(const Rect& size, Texture2D::Type type)
{
	return std::make_shared<Texture2D>(size, type, this);
}

void RenderSystem::initRasterizerState()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;

	HRESULT hr = m_d3d_device->CreateRasterizerState(&desc, &m_cull_front_state);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] Create Cull front state creation failed.");

	desc.CullMode = D3D11_CULL_BACK;

	hr = m_d3d_device->CreateRasterizerState(&desc, &m_cull_back_state);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] Create Cull back state creation failed.");
}

void RenderSystem::compilePrivateShaders()
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3DBlob> errBlob;

	auto meshLayoutCode = R"(
struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    return output;
}
	)";

	auto codeSize = std::strlen(meshLayoutCode);

	if (FAILED(D3DCompile(meshLayoutCode, codeSize, "VertexMeshLayoutShader", nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &errBlob)))
	{
		Dx3DError("VertexMeshLayoutShader compilation failed.");
	}

	memcpy(m_meshLayoutByteCode, blob->GetBufferPointer(), blob->GetBufferSize());
	m_meshLayoutSize = blob->GetBufferSize();
}

bool RenderSystem::compileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size)
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

bool RenderSystem::compilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shader_byte_code, size_t* byte_code_size)
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

void RenderSystem::releaseCompiledShader()
{
	if (this->m_blob)
		this->m_blob->Release();
}

void RenderSystem::setRasterizerState(bool cull_front)
{
	if (cull_front)
	{
		m_deviceContext->m_deviceContext->RSSetState(m_cull_front_state.Get());
	}
	else
	{
		m_deviceContext->m_deviceContext->RSSetState(m_cull_back_state.Get());
	}
}
