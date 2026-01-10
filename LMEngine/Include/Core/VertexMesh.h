#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VertexMesh
{
public:
	VertexMesh() : m_position(), m_texCoord() {}
	VertexMesh(Vector3D pos, Vector2D tcoord) : m_position(pos), m_texCoord(tcoord) {}
	VertexMesh(const VertexMesh& vec) : m_position(vec.m_position), m_texCoord(vec.m_texCoord) {}

	~VertexMesh()
	{
	}

public:
	Vector3D m_position;
	Vector2D m_texCoord;
};