#pragma once
#include <d3d11.h>
#include "Vec4.h"
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system);
	~DeviceContext();

public:
	void ClearRenderTarget(SwapChainPtr swapChain, Vec4 color);

	void SetVertexBuffer(VertexBufferPtr vertex_buffer);
	void SetIndexBuffer(IndexBufferPtr index_buffer);

	void SetViewPortSize(UINT width, UINT height);

	void SetVertexShader(VertexShaderPtr vertex_shader);
	void SetPixelShader(PixelShaderPtr pixel_shader);

	void SetConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr constant_buffer);
	void SetConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr constant_buffer);

	void DrawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void DrawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

private:
	ID3D11DeviceContext* m_deviceContext;

	RenderSystem* m_system = nullptr;

private:
	friend class GraphicEngine;
	friend class ConstantBuffer;
};

