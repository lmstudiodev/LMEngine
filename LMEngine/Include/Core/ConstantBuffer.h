#pragma once
#include <Prerequisites.h>

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system);

public:
	void Update(DeviceContextPtr context, void* buffer);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

