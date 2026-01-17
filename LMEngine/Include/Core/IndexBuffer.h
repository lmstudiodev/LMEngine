#pragma once
#include <Prerequisites.h>

class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);

public:
	UINT getSizeIndexList();

private:
	UINT m_size_list;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

