#include <stdafx.h>
#include <Core/ConstantBuffer.h>
#include <Core/RenderSystem.h>
#include <Core/DeviceContext.h>

ConstantBuffer::ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system) : m_buffer(nullptr), m_system(system)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = size_buffer;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = buffer;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
	{
		Dx3DError("ConstantBuffer creation failed.");
	}
}

void ConstantBuffer::Update(const DeviceContextPtr& context, void* buffer)
{
	context->m_deviceContext->UpdateSubresource(this->m_buffer.Get(), NULL, NULL, buffer, NULL, NULL);
}