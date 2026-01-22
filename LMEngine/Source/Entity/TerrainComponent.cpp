#include <stdafx.h>
#include <Entity/TerrainComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Core/GraphicEngine.h>
#include <Core//RenderSystem.h>
#include <Core/ConstantBuffer.h>
#include <Math/VertexMesh.h>

TerrainComponent::TerrainComponent()
{
}

TerrainComponent::~TerrainComponent()
{
	m_entity->getWorld()->getGame()->getGraphicEngine()->removeComponent(this);
}

void TerrainComponent::setMesh(const MeshPtr& mesh)
{
	m_mesh = mesh;
}

const MeshPtr& TerrainComponent::getMesh()
{
	return m_mesh;
}

void TerrainComponent::setHeightMap(const TexturePtr& heightMap)
{
	m_heightMap = heightMap;
}

const TexturePtr& TerrainComponent::getHeightMap()
{
	return m_heightMap;
}

void TerrainComponent::setSize(const Vector3D& size)
{
	m_size = size;
}

const Vector3D TerrainComponent::getSize()
{
	return m_size;
}

void TerrainComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicEngine()->addComponent(this);

	generateTerrainMesh();
}

void TerrainComponent::generateTerrainMesh()
{
	const ui32 w = 512;
	const ui32 h = 512;

	const ui32 ww = w - 1;
	const ui32 hh = h - 1;

	VertexMesh* terrainMeshVertices = new VertexMesh[w * h];
	ui32* terrainMeshIndices = new ui32[ww * hh * 6];

	auto i = 0;

	for (ui32 x = 0; x < w; x++)
	{
		for (ui32 y = 0; y < h; y++)
		{
			terrainMeshVertices[x * y + x] = {
				Vector3D((f32)x / (f32)ww, 0, (f32)y / (f32)hh),
				Vector2D((f32)x / (f32)ww,(f32)y / (f32)hh),
				Vector3D(),
				Vector3D(),
				Vector3D()
			};

			if (x < ww && y < hh)
			{
				terrainMeshIndices[i] = (y + 1) * w + (x);
				terrainMeshIndices[i + 1] = (y) * w + (x);
				terrainMeshIndices[i + 2] = (y) * w + (x + 1);

				terrainMeshIndices[i + 3] = (y)*w + (x + 1);
				terrainMeshIndices[i + 4] = (y + 1) * w + (x + 1);
				terrainMeshIndices[i + 5] = (y + 1) * w + (x);

				i += 6;
			}
		}
	}

	auto rs = m_entity->getWorld()->getGame()->getGraphicEngine()->getRenderSystem();

	m_meshVB = rs->createVertexBuffer(terrainMeshVertices, sizeof(VertexMesh), w * h);
	m_meshIB = rs->createIndexBuffer(terrainMeshIndices, ww * hh * 6);

	m_vs = rs->createVertexShader(L"Resources/Shader/Terrain.hlsl", "vsmain");
	m_ps = rs->createPixelShader(L"Resources/Shader/Terrain.hlsl", "psmain");
}

void TerrainComponent::updateData(void* data, ui32 size)
{
	auto rs = m_entity->getWorld()->getGame()->getGraphicEngine()->getRenderSystem();

	if (!m_cb)
	{
		m_cb = rs->createConstantBuffer(data, size);
	}
	else
	{
		m_cb->Update(rs->getDeviceContext(), data);
	}
}
