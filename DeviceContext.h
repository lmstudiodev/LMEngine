#pragma once
#include <d3d11.h>
#include "Vec4.h"

class SwapChain;
class VertexBuffer;
class IndexBuffer;
class GraphicEngine;
class VertexShader;
class PixelShader;
class ConstantBuffer;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext);
	~DeviceContext();

public:
	void ClearRenderTarget(SwapChain* swapChain, Vec4 color);

	void SetVertexBuffer(VertexBuffer* vertex_buffer);
	void SetIndexBuffer(IndexBuffer* index_buffer);

	void SetViewPortSize(UINT width, UINT height);

	void SetVertexShader(VertexShader* vertex_shader);
	void SetPixelShader(PixelShader* pixel_shader);

	void SetConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* constant_buffer);
	void SetConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* constant_buffer);

	void DrawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void DrawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	bool Release();

private:
	ID3D11DeviceContext* m_deviceContext;

private:
	friend class GraphicEngine;
	friend class ConstantBuffer;
};

