#include "stdafx.h"
#include "VertexBuffer.h"
#include "RenderSystem.h"

VertexBuffer::VertexBuffer(void* list_vertices, 
	UINT size_vertex, 
	UINT size_list, 
	void* shader_byte_code, 
	UINT size_byte_shader, 
	RenderSystem* system) : m_buffer(0), m_size_vertex(0), m_size_list(0), m_layout(0), m_system(system)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = size_vertex * size_list;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
		throw std::exception("[D3D11 Error] VertexBuffer creation failed.");

	D3D11_INPUT_ELEMENT_DESC elementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT size_layout = ARRAYSIZE(elementDesc);

	if (FAILED(m_system->m_d3d_device->CreateInputLayout(elementDesc, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
		throw std::exception("[D3D11 Error] InputLayout creation failed.");
}

VertexBuffer::~VertexBuffer()
{
	if(m_layout)
		m_layout->Release();

	if(m_buffer)
		m_buffer->Release();
}

UINT VertexBuffer::GetSizeVertexList()
{
	return this->m_size_list;
}