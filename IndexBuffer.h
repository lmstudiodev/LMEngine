#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer
{
public:
	IndexBuffer(RenderSystem* system);
	~IndexBuffer();

public:
	bool Load(void* list_indices, UINT size_list);
	UINT GetSizeIndexList();
	bool Release();

private:
	UINT m_size_list;
	ID3D11Buffer* m_buffer;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

