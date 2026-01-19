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
#include <Entity/Entity.h>
#include <Entity/TransformComponent.h>
#include <Entity/MeshComponent.h>

__declspec(align(16))
struct ConstantData
{
	Matrix4x4 m_worldMatrix;
	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projectionMatrix;
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

	context->clearRenderTarget(swapchain, { 1.0f, 0.0f, 0.0f, 1.0f });

	auto winSize = m_game->m_display->getClientSize();

	if (winSize.width <= 0 || winSize.height <= 0)
		return;

	context->setViewPortSize(winSize.width, winSize.height);

	ConstantData constData{};

	constData.m_viewMatrix.SetIdentity();
	constData.m_projectionMatrix.SetIdentity();

	constData.m_viewMatrix.SetTranslation(Vector3D(0.0f, 0.0f, -10.0f));
	constData.m_viewMatrix.inverse();

	constData.m_projectionMatrix.SetPerspectiveFovLH(1.5f, (float)winSize.width / (float)winSize.height, 0.01f, 1000.0f);

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
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.emplace(c);
}

void GraphicEngine::removeComponent(Component* component)
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.erase(c);
}

