#pragma once
#include "Vec4.h"
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system);
	~DeviceContext();

public:
	void ClearRenderTarget(const SwapChainPtr& swapChain, Vec4 color);
	void ClearDepthStencil(const SwapChainPtr& swapChain);

	void ClearRenderTarget(const TexturePtr& rendertarget, Vec4 color);
	void ClearDepthStencil(const TexturePtr& depthStencil);

	void SetRenderTarget(const TexturePtr& rendertarget, const TexturePtr& depthStencil);

	void SetVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void SetIndexBuffer(const IndexBufferPtr& index_buffer);

	void SetViewPortSize(UINT width, UINT height);

	void SetVertexShader(const VertexShaderPtr& vertex_shader);
	void SetPixelShader(const PixelShaderPtr& pixel_shader);

	void SetTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures);
	void SetTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures);

	void SetConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& constant_buffer);
	void SetConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer);

	void DrawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void DrawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

private:
	ID3D11DeviceContext* m_deviceContext;

	RenderSystem* m_system = nullptr;

private:
	friend class GraphicEngine;
	friend class ConstantBuffer;
	friend class RenderSystem;
};

