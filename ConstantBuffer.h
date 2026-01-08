#pragma once
#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

public:
	bool Load(void* buffer, UINT size_buffer);
	void Update(DeviceContext* context, void* buffer);
	bool Release();

private:
	ID3D11Buffer* m_buffer;

private:
	friend class DeviceContext;
};

