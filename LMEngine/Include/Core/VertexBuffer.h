#pragma once
#include <Prerequisites.h>

class VertexBuffer
{
public:
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, RenderSystem* system);

public:
	UINT getSizeVertexList();

private:
	UINT m_size_vertex;
	UINT m_size_list;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

