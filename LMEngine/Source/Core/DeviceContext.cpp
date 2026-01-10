#include "stdafx.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system) : m_deviceContext(deviceContext), m_system(system)
{
}

DeviceContext::~DeviceContext()
{
	if(m_deviceContext)
		m_deviceContext->Release();
}

void DeviceContext::ClearRenderTarget(const SwapChainPtr& swapChain, Vec4 color)
{
	const float colorValues[] = { color.x, color.y, color.z, color.w };

	m_deviceContext->ClearRenderTargetView(swapChain->m_rtv, colorValues);
	m_deviceContext->ClearDepthStencilView(swapChain->m_dsw, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	m_deviceContext->OMSetRenderTargets(1, &swapChain->m_rtv, swapChain->m_dsw);
}

void DeviceContext::SetVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	
	m_deviceContext->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_deviceContext->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::SetIndexBuffer(const IndexBufferPtr& index_buffer)
{
	UINT offset = 0;
	m_deviceContext->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, offset);
}

void DeviceContext::DrawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::DrawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_deviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::SetViewPortSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp{};
	vp.Width = FLOAT(width);
	vp.Height = FLOAT(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	
	m_deviceContext->RSSetViewports(1, &vp);
}

void DeviceContext::SetVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_deviceContext->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::SetPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_deviceContext->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

void DeviceContext::SetTexture(const VertexShaderPtr& vertex_shader, const TexturePtr& texture)
{
	m_deviceContext->VSSetShaderResources(0, 1, &texture->m_shaderResource);
}

void DeviceContext::SetTexture(const PixelShaderPtr& pixel_shader, const TexturePtr& texture)
{
	m_deviceContext->PSSetShaderResources(0, 1, &texture->m_shaderResource);
}

void DeviceContext::SetConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& constant_buffer)
{
	m_deviceContext->VSSetConstantBuffers(0, 1, &constant_buffer->m_buffer);
}

void DeviceContext::SetConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer)
{
	m_deviceContext->PSSetConstantBuffers(0, 1, &constant_buffer->m_buffer);
}
