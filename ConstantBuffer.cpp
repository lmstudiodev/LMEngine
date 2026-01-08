#include "ConstantBuffer.h"
#include "GraphicEngine.h"
#include "DeviceContext.h"

ConstantBuffer::ConstantBuffer() : m_buffer(nullptr)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

bool ConstantBuffer::Load(void* buffer, UINT size_buffer)
{
	if (m_buffer)
		m_buffer->Release();

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = size_buffer;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = buffer;

	if (FAILED(GraphicEngine::Get()->m_d3d_device->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
		return false;

	return true;
}

void ConstantBuffer::Update(DeviceContext* context, void* buffer)
{
	context->m_deviceContext->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);
}

bool ConstantBuffer::Release()
{
	m_buffer->Release();

	delete this;

	return true;
}