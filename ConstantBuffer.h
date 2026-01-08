#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(RenderSystem* system);
	~ConstantBuffer();

public:
	bool Load(void* buffer, UINT size_buffer);
	void Update(DeviceContext* context, void* buffer);
	bool Release();

private:
	ID3D11Buffer* m_buffer;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

