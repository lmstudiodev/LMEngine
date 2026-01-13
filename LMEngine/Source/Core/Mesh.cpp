#include "stdafx.h"
#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "GraphicEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path), m_vertex_buffer(nullptr), m_index_buffer(nullptr)
{
	tinyobj::attrib_t attributes{};
	
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	std::string inputfile = std::filesystem::path(full_path).string();

	std::string mtldir = inputfile.substr(0, inputfile.find_last_of("\\/"));
	
	bool result = tinyobj::LoadObj(&attributes, 
									&shapes,
									&materials,
									&warn, 
									&err, 
									inputfile.c_str(),
									mtldir.c_str());

	if (!err.empty())
	{
		std::cout << "[OBJ LOADER Error] " << err << "\n";
		throw std::exception("[OBJ LOADER Exception] Load model from file failed.");
	}

	if (!warn.empty())
		std::cout << "[OBJ LOADER Warning] " << warn << "\n";

	if (!result)
		throw std::exception("[OBJ LOADER Ecxeption] Load model from file failed.");

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;


	size_t size_vertex_index_list = 0;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_vertex_index_list += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(size_vertex_index_list);
	list_indices.reserve(size_vertex_index_list);

	m_material_slots.resize(materials.size());

	size_t index_global_offset = 0;

	try
	{
		for (size_t m = 0; m < materials.size(); m++)
		{			
			m_material_slots[m].start_index = index_global_offset;
			m_material_slots[m].material_id = m;

			for (size_t s = 0; s < shapes.size(); s++)
			{				
				size_t indexOffset = 0;

				list_vertices.reserve(shapes[s].mesh.indices.size());
				list_indices.reserve(shapes[s].mesh.indices.size());

				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
				{
					
					if (shapes[s].mesh.material_ids[f] != m)
						continue;

					unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

					for (unsigned char v = 0; v < num_face_verts; v++)
					{						
						tinyobj::index_t index = shapes[s].mesh.indices[indexOffset + v];

						tinyobj::real_t vx = attributes.vertices[index.vertex_index * 3 + 0];
						tinyobj::real_t vy = attributes.vertices[index.vertex_index * 3 + 1];
						tinyobj::real_t vz = attributes.vertices[index.vertex_index * 3 + 2];

						tinyobj::real_t tx = attributes.texcoords[index.texcoord_index * 2 + 0];
						tinyobj::real_t ty = attributes.texcoords[index.texcoord_index * 2 + 1];

						tinyobj::real_t nx = attributes.normals[index.normal_index * 3 + 0];
						tinyobj::real_t ny = attributes.normals[index.normal_index * 3 + 1];
						tinyobj::real_t nz = attributes.normals[index.normal_index * 3 + 2];

						VertexMesh vm(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));

						list_vertices.push_back(vm);

						list_indices.push_back((unsigned int)index_global_offset + v);\
					}

					indexOffset += num_face_verts;
					index_global_offset += num_face_verts;
				}
			}

			m_material_slots[m].num_indices = index_global_offset - m_material_slots[m].start_index;
		}
	}
	catch (...)
	{
		std::cout << "...." << "\n";
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicEngine::Get()->GetVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	m_vertex_buffer = GraphicEngine::Get()->GetRenderSystem()->CreateVertexBuffer(&list_vertices[0], sizeof(VertexMesh), (UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);

	m_index_buffer = GraphicEngine::Get()->GetRenderSystem()->CreateIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::GetVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::GetIndexBuffer()
{
	return m_index_buffer;
}

const MaterialSlot& Mesh::GetMaterialSlot(unsigned int slot)
{
	if (slot >= m_material_slots.size())
		return MaterialSlot();

	return m_material_slots[slot];
}

size_t Mesh::GetNumMaterialSlots()
{
	return m_material_slots.size();
}
