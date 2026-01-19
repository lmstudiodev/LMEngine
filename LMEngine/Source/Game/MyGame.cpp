#include <stdafx.h>
#include <Game/MyGame.h>
#include <Game/Player.h>

MyGame::MyGame()
{

}

MyGame::~MyGame()
{
}

void MyGame::onUpdate(f32 deltaTime)
{
	Game::onUpdate(deltaTime);

	m_rotation += 1.57f * deltaTime;

	//m_entity->getTransformComponent()->setRotation(Vector3D(0.0f, m_rotation, 0.0f));

	if (getInputSystem()->isKeyUp(Key::Escape))
	{
		m_locked = !m_locked;
		getInputSystem()->lockMouseCursor(m_locked);
	}
}

void MyGame::onCreate()
{
	Game::onCreate();

	auto house_mesh = getResourceManager()->createResourceFromFile<Mesh>(L"Resources/Meshes/house.obj");
	auto terrain_mesh = getResourceManager()->createResourceFromFile<Mesh>(L"Resources/Meshes/terrain.obj");


	auto house_texture = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/wood.jpg");
	auto terrain_texture = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/Sponza/sponza_floor_a_diff.jpg");



	auto house_material = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	house_material->AddTexture(house_texture);

	auto terrain_material = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	terrain_material->AddTexture(terrain_texture);

	{
		auto entity = getWorld()->createEntity<Entity>();
		auto meshComponent = entity->createComponent<MeshComponent>();
		meshComponent->setMesh(terrain_mesh);
		meshComponent->addMaterial(terrain_material);
	}


	m_entity = getWorld()->createEntity<Entity>();
	auto meshComponent = m_entity->createComponent<MeshComponent>();
	meshComponent->setMesh(house_mesh);
	meshComponent->addMaterial(house_material);
	meshComponent->addMaterial(house_material);
	meshComponent->addMaterial(house_material);
	meshComponent->addMaterial(house_material);

	getWorld()->createEntity<Player>();

	getInputSystem()->lockMouseCursor(m_locked);
}