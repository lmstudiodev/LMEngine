#include <stdafx.h>
#include <IndexBuffer.h>
#include <RenderSystem.h>

IndexBuffer::IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system) : m_buffer(nullptr), m_size_list(0), m_system(system)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 4 * size_list;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = list_indices;

	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
	{
		throw std::exception("[D3D11 Error] IndexBuffer creation failed.");
	}
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_size_list;
}
