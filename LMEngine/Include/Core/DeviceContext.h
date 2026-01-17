#pragma once
#include <Vec4.h>
#include <Prerequisites.h>

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system);

public:
	void clearRenderTarget(const SwapChainPtr& swapChain, Vec4 color);
	void clearDepthStencil(const SwapChainPtr& swapChain);

	void clearRenderTarget(const TexturePtr& rendertarget, Vec4 color);
	void clearDepthStencil(const TexturePtr& depthStencil);

	void setRenderTarget(const TexturePtr& rendertarget, const TexturePtr& depthStencil);

	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr& index_buffer);

	void setViewPortSize(UINT width, UINT height);

	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);

	void setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures);
	void setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures);

	void setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& constant_buffer);
	void setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;

	RenderSystem* m_system = nullptr;

private:
	friend class GraphicEngine;
	friend class ConstantBuffer;
	friend class RenderSystem;
};

