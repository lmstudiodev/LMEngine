#pragma once
#include <Prerequisites.h>
#include <Entity/Component.h>
#include <Math/Vector3D.h>

class TerrainComponent : public Component
{
public:
	TerrainComponent();
	virtual ~TerrainComponent();

public:
	void setMesh(const MeshPtr& mesh);
	const MeshPtr& getMesh();

	void setHeightMap(const TexturePtr& heightMap);
	const TexturePtr& getHeightMap();

	void setSize(const Vector3D& size);
	const Vector3D getSize();

protected:
	virtual void onCreateInternal();

private:
	void generateTerrainMesh();
	void updateData(void* data, ui32 size);

private:
	MeshPtr m_mesh;
	TexturePtr m_heightMap;
	VertexBufferPtr m_meshVB;
	IndexBufferPtr m_meshIB;
	ConstantBufferPtr m_cb;

	Vector3D m_size = Vector3D(512, 100, 512);

	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;

private:
	friend class GraphicEngine;
};

