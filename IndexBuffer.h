#pragma once
#include <d3d11.h>

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

public:
	bool Load(void* list_indices, UINT size_list);
	UINT GetSizeIndexList();
	bool Release();

private:
	UINT m_size_list;
	ID3D11Buffer* m_buffer;

private:
	friend class DeviceContext;
};

