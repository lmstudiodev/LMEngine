#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VertexMesh
{
public:
	VertexMesh() : m_position(), m_texCoord(), m_normal(), m_tangent(), m_binormal() {}

	VertexMesh(const Vector3D& pos, 
		const Vector2D& tcoord,
		const Vector3D& norm,
		const Vector3D& tangent,
		const Vector3D& binormal) : 
		m_position(pos), 
		m_texCoord(tcoord), 
		m_normal(norm), 
		m_tangent(tangent), 
		m_binormal(binormal) {}


	VertexMesh(const VertexMesh& vec) : 
		m_position(vec.m_position), 
		m_texCoord(vec.m_texCoord), 
		m_normal(vec.m_normal),
		m_tangent(vec.m_tangent),
		m_binormal(vec.m_binormal) {}

	~VertexMesh()
	{
	}

public:
	Vector3D m_position;
	Vector2D m_texCoord;
	Vector3D m_normal;
	Vector3D m_tangent;
	Vector3D m_binormal;
};