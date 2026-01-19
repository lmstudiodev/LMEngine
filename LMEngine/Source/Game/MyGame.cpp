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

	m_entity->getTransformComponent()->setRotation(Vector3D(0.0f, m_rotation, 0.0f));
}

void MyGame::onCreate()
{
	Game::onCreate();

	auto mesh = getResourceManager()->createResourceFromFile<Mesh>(L"Resources/Meshes/house.obj");
	auto texture = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/wood.jpg");
	auto material = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	material->AddTexture(texture);

	m_entity = getWorld()->createEntity<Entity>();
	auto meshComponent = m_entity->createComponent<MeshComponent>();
	meshComponent->setMesh(mesh);
	meshComponent->addMaterial(material);
	meshComponent->addMaterial(material);
	meshComponent->addMaterial(material);
	meshComponent->addMaterial(material);

	getWorld()->createEntity<Player>();
}