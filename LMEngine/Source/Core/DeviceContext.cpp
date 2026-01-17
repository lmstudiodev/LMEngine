#include "stdafx.h"
#include <DeviceContext.h>
#include "SwapChain.h"
#include "VertexBuffer.h"
#include <IndexBuffer.h>
#include <ConstantBuffer.h>
#include <VertexShader.h>
#include <PixelShader.h>
#include <Texture.h>

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system) : m_deviceContext(deviceContext), m_system(system)
{
}

void DeviceContext::clearRenderTarget(const SwapChainPtr& swapChain, Vec4 color)
{
	const float colorValues[] = { color.x, color.y, color.z, color.w };

	m_deviceContext->ClearRenderTargetView(swapChain->m_rtv.Get(), colorValues);
	m_deviceContext->ClearDepthStencilView(swapChain->m_dsw.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	m_deviceContext->OMSetRenderTargets(1, &swapChain->m_rtv, swapChain->m_dsw.Get());
}

void DeviceContext::clearDepthStencil(const SwapChainPtr& swapChain)
{
	m_deviceContext->ClearDepthStencilView(swapChain->m_dsw.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DeviceContext::clearRenderTarget(const TexturePtr& rendertarget, Vec4 color)
{
	if (rendertarget->m_type != Texture::Type::RenderTarget)
		return;

	const float colorValues[] = { color.x, color.y, color.z, color.w };

	m_deviceContext->ClearRenderTargetView(rendertarget->m_renderTargetView, colorValues);
}

void DeviceContext::clearDepthStencil(const TexturePtr& depthStencil)
{
	if (depthStencil->m_type != Texture::Type::DepthStencil)
		return;

	m_deviceContext->ClearDepthStencilView(depthStencil->m_depthStenciltView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DeviceContext::setRenderTarget(const TexturePtr& rendertarget, const TexturePtr& depthStencil)
{
	if (rendertarget->m_type != Texture::Type::RenderTarget)
		return;

	if (depthStencil->m_type != Texture::Type::DepthStencil)
		return;

	m_deviceContext->OMSetRenderTargets(1, &rendertarget->m_renderTargetView, depthStencil->m_depthStenciltView);
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	
	m_deviceContext->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_deviceContext->IASetInputLayout(vertex_buffer->m_layout.Get());
}

void DeviceContext::setIndexBuffer(const IndexBufferPtr& index_buffer)
{
	UINT offset = 0;
	m_deviceContext->IASetIndexBuffer(index_buffer->m_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_deviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::setViewPortSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp{};
	vp.Width = FLOAT(width);
	vp.Height = FLOAT(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	
	m_deviceContext->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_deviceContext->VSSetShader(vertex_shader->m_vs.Get(), nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_deviceContext->PSSetShader(pixel_shader->m_ps.Get(), nullptr, 0);
}

void DeviceContext::setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures)
{
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_samplers[32];

	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shaderResource;
		list_samplers[i] = texture[i]->m_samplerState;
	}

	m_deviceContext->VSSetShaderResources(0, num_textures, list_res);
	m_deviceContext->VSSetSamplers(0, num_textures, list_samplers);
}

void DeviceContext::setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures)
{
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_samplers[32];

	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shaderResource;
		list_samplers[i] = texture[i]->m_samplerState;
	}

	m_deviceContext->PSSetShaderResources(0, num_textures, list_res);
	m_deviceContext->PSSetSamplers(0, num_textures, list_samplers);
}

void DeviceContext::setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& constant_buffer)
{
	m_deviceContext->VSSetConstantBuffers(0, 1, &constant_buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer)
{
	m_deviceContext->PSSetConstantBuffers(0, 1, &constant_buffer->m_buffer);
}
