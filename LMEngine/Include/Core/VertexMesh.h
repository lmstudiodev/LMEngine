#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VertexMesh
{
public:
	VertexMesh() : m_position(), m_texCoord(), m_normal() {}
	VertexMesh(Vector3D pos, Vector2D tcoord, Vector3D norm) : m_position(pos), m_texCoord(tcoord), m_normal(norm) {}
	VertexMesh(const VertexMesh& vec) : m_position(vec.m_position), m_texCoord(vec.m_texCoord), m_normal(vec.m_normal) {}

	~VertexMesh()
	{
	}

public:
	Vector3D m_position;
	Vector2D m_texCoord;
	Vector3D m_normal;
};