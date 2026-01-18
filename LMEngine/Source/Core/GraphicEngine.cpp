#include <stdafx.h>
#include <GraphicEngine.h>
#include <SwapChain.h>
#include <ResourceManager.h>
#include <Mesh.h>
#include <Texture.h>
#include <Material.h>
#include <RenderSystem.h>
#include <DeviceContext.h>
#include <Game.h>
#include <Display.h>
#include <Matrix4x4.h>

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

void GraphicEngine::update(const MeshData& data)
{
	auto swapchain = m_game->m_display->m_swapChain;

	auto context = m_render_system->getDeviceContext();

	context->clearRenderTarget(swapchain, { 1.0f, 0.0f, 0.0f, 1.0f });

	auto winSize = m_game->m_display->getClientSize();

	if (winSize.width <= 0 || winSize.height <= 0)
		return;

	context->setViewPortSize(winSize.width, winSize.height);

	ConstantData constData{};
	constData.m_worldMatrix.SetIdentity();
	constData.m_viewMatrix.SetIdentity();
	constData.m_projectionMatrix.SetIdentity();

	constData.m_worldMatrix.SetRotationY(0.707f);

	constData.m_viewMatrix.SetTranslation(Vector3D(0.0f, 0.0f, -10.0f));
	constData.m_viewMatrix.inverse();

	constData.m_projectionMatrix.SetPerspectiveFovLH(1.5f, (float)winSize.width / (float)winSize.height, 0.01f, 1000.0f);

	context->setVertexBuffer(data.mesh->m_vertex_buffer);
	context->setIndexBuffer(data.mesh->m_index_buffer);

	for (auto i = 0; i < data.mesh->getNumMaterialSlots(); i++)
	{
		data.material->SetData(&constData, sizeof(ConstantData));
		
		context->setConstantBuffer(data.material->m_constant_buffer);

		context->setVertexShader(data.material->m_vertex_shader);
		context->setPixelShader(data.material->m_pixel_shader);

		context->setTexture(&data.material->m_textures[0], data.material->m_textures.size());

		auto slot = data.mesh->getMaterialSlot(i);

		context->drawIndexedTriangleList(slot.num_indices, slot.start_index, 0);
	}

	swapchain->present(true);
}

RenderSystem* GraphicEngine::getRenderSystem()
{
	return m_render_system.get();
}

