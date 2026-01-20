#include <stdafx.h>
#include <Core/GraphicEngine.h>
#include <Core/SwapChain.h>
#include <ResourceManagers/ResourceManager.h>
#include <ResourceManagers/Mesh.h>
#include <ResourceManagers/Texture.h>
#include <ResourceManagers/Material.h>
#include <Core/RenderSystem.h>
#include <Core/DeviceContext.h>
#include <Game/Game.h>
#include <Game/Display.h>
#include <Math/Matrix4x4.h>
#include <Math/Vector4D.h>
#include <Entity/Entity.h>
#include <Entity/TransformComponent.h>
#include <Entity/MeshComponent.h>
#include <Entity/CameraComponent.h>
#include <Entity/LightComponent.h>

__declspec(align(16))
struct LightData
{
	Vector4D color;
	Vector4D direction;
};

__declspec(align(16))
struct ConstantData
{
	Matrix4x4 m_worldMatrix;
	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projectionMatrix;
	Vector4D m_cameraPosition;
	LightData m_lightData;
};

GraphicEngine::GraphicEngine(Game* game) : m_game(game), m_render_system(nullptr)
{
	m_render_system = std::make_unique<RenderSystem>();
}

GraphicEngine::~GraphicEngine()
{
}

void GraphicEngine::update()
{
	auto swapchain = m_game->m_display->m_swapChain;

	auto context = m_render_system->getDeviceContext();

	context->clearRenderTarget(swapchain, { 0.0f, 0.0f, 0.0f, 1.0f });

	auto winSize = m_game->m_display->getClientSize();

	if (winSize.width <= 0 || winSize.height <= 0)
		return;

	context->setViewPortSize(winSize.width, winSize.height);

	ConstantData constData{};

	constData.m_viewMatrix.SetIdentity();
	constData.m_projectionMatrix.SetIdentity();

	for (auto c : m_cameras)
	{
		auto t = c->getEntity()->getTransformComponent();
		
		constData.m_cameraPosition = t->getPosition();
		c->setScreenArea(winSize);
		c->getViewmatrix(constData.m_viewMatrix);
		c->getProjectionMatrix(constData.m_projectionMatrix);
	}

	for (auto l : m_lights)
	{
		auto t = l->getEntity()->getTransformComponent();

		Matrix4x4 w;
		t->getWorldMatrix(w);
		
		constData.m_lightData.direction = w.GetZDirection();
		constData.m_lightData.color = l->getColor();
	}

	for (auto m : m_meshes)
	{
		auto transform =  m->getEntity()->getTransformComponent();
		transform->getWorldMatrix(constData.m_worldMatrix);

		auto mesh = m->getMesh().get();
		const auto materials = m->getMaterials();
		
		context->setVertexBuffer(mesh->m_vertex_buffer);
		context->setIndexBuffer(mesh->m_index_buffer);

		for (auto i = 0; i < mesh->getNumMaterialSlots(); i++)
		{
			if (i >= materials.size())
				break;

			auto mat = materials[i].get();

			m_render_system->setCullMode(mat->GetCullMode());
			
			mat->SetData(&constData, sizeof(ConstantData));

			context->setConstantBuffer(mat->m_constant_buffer);

			context->setVertexShader(mat->m_vertex_shader);
			context->setPixelShader(mat->m_pixel_shader);

			context->setTexture(&mat->m_textures[0], mat->m_textures.size());

			auto slot = mesh->getMaterialSlot(i);

			context->drawIndexedTriangleList(slot.num_indices, slot.start_index, 0);
		}
	}

	swapchain->present(true);
}

RenderSystem* GraphicEngine::getRenderSystem()
{
	return m_render_system.get();
}

void GraphicEngine::addComponent(Component* component)
{
	if (auto m = dynamic_cast<MeshComponent*>(component))
		m_meshes.emplace(m);

	if (auto c = dynamic_cast<CameraComponent*>(component))
	{
		if(!m_cameras.size())
			m_cameras.emplace(c);
	}

	if (auto l = dynamic_cast<LightComponent*>(component))
	{
		if (!m_lights.size())
			m_lights.emplace(l);
	}
}

void GraphicEngine::removeComponent(Component* component)
{
	if (auto m = dynamic_cast<MeshComponent*>(component))
		m_meshes.erase(m);

	if (auto c = dynamic_cast<CameraComponent*>(component))
		m_cameras.erase(c);

	if (auto l = dynamic_cast<LightComponent*>(component))
		m_lights.erase(l);
}

