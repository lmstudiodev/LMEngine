#pragma once
#include "Global.h"
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system);
	~ConstantBuffer();

public:
	void Update(DeviceContextPtr context, void* buffer);

private:
	ID3D11Buffer* m_buffer;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

