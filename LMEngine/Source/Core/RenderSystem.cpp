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
		Dx3DError("D3D11CreateDevice creation failed.");

	m_deviceContext = std::make_shared<DeviceContext>(m_immediateDeviceContext.Get(), this);

	if (FAILED(m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice)))
		Dx3DError("IDXGIDevice creation failed.");

	if (FAILED(m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter)))
		Dx3DError("IDXGIAdapter creation failed.");

	if (FAILED(m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory)))
		Dx3DError("IDXGIFactory creation failed.");

	initRasterizerState();
	compilePrivateShaders();
}

RenderSystem::~RenderSystem()
{

}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	return std::make_shared<SwapChain>(hwnd, width, height, this);
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
	return std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	return std::make_shared<IndexBuffer>(list_indices, size_list, this);
}

VertexShaderPtr RenderSystem::createVertexShader(const wchar_t* full_path, const char* entryPoint)
{
	return std::make_shared<VertexShader>(full_path, entryPoint, this);
}

PixelShaderPtr RenderSystem::createPixelShader(const wchar_t* full_path, const char* entryPoint)
{
	return std::make_shared<PixelShader>(full_path, entryPoint, this);
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
		Dx3DError("Create Cull front state creation failed.");

	desc.CullMode = D3D11_CULL_BACK;

	hr = m_d3d_device->CreateRasterizerState(&desc, &m_cull_back_state);

	if (FAILED(hr))
		Dx3DError("Create Cull back state creation failed.");
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
