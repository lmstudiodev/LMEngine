#include "ConstantBuffer.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include <exception>

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
		throw std::exception("[D3D11 Error] ConstantBuffer creation failed.");
	}
}

ConstantBuffer::~ConstantBuffer()
{
	if(m_buffer)
		m_buffer->Release();
}

void ConstantBuffer::Update(DeviceContext* context, void* buffer)
{
	context->m_deviceContext->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);
}