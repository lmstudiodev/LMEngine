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

	if (m_entity)
	{
		m_rotation += 1.57f * deltaTime;

		m_entity->getTransformComponent()->setRotation(Vector3D(-0.707f, m_rotation, 0.0f));
	}

	if (getInputSystem()->isKeyUp(Key::Escape))
	{
		m_locked = !m_locked;
		getInputSystem()->lockMouseCursor(m_locked);
	}
}

void MyGame::onCreate()
{
	Game::onCreate();

	srand((ui32)time(nullptr));

	//MESH
	//auto house_mesh = getResourceManager()->createResourceFromFile<Mesh>(L"Resources/Meshes/house.obj");
	//auto terrain_mesh = getResourceManager()->createResourceFromFile<Mesh>(L"Resources/Meshes/terrain.obj");
	auto sky_mesh = getResourceManager()->createResourceFromFile<Mesh>(L"Resources/Meshes/sphere.obj");
	//auto box_mesh = getResourceManager()->createResourceFromFile<Mesh>(L"Resources/Meshes/box.obj");
	//auto sponza_mesh = getResourceManager()->createResourceFromFile<Mesh>(L"Resources/Meshes/sponza_basic.obj");

	//TEXTURE
	//auto wood_texture = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/wood.jpg");
	//auto terrain_texture = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/Sponza/sponza_floor_a_diff.jpg");
	auto sky_texture = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/sky.jpg");
	auto terrain_heighmap = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/height_map.png");

	//auto sponza_brick = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/Sponza/sponza_bricks_a_diff.jpg");
	//auto sponza_arch = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/Sponza/sponza_arch_diff.jpg");
	//auto sponza_column_a = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/Sponza/sponza_column_a_diff.jpg");
	//auto sponza_column_b = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/Sponza/sponza_column_b_diff.jpg");
	//auto sponza_column_c = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/Sponza/sponza_column_c_diff.jpg");
	//auto sponza_flag_pole = getResourceManager()->createResourceFromFile<Texture>(L"Resources/Textures/Sponza/sponza_flagpole_diff.jpg");

	//MATERIAL
	//auto wood_material = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	//wood_material->AddTexture(wood_texture);

	//auto terrain_material = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	//terrain_material->AddTexture(terrain_texture);

	auto sky_material = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/SkyBox.hlsl");
	sky_material->AddTexture(sky_texture);
	sky_material->SetCullMode(CullMode::Front);

	//auto sponza_material_brick = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	//sponza_material_brick->AddTexture(sponza_brick);
	//auto sponza_material_arch = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	//sponza_material_arch->AddTexture(sponza_arch);
	//auto sponza_material_col_a = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	//sponza_material_col_a->AddTexture(sponza_column_a);
	//auto sponza_material_col_b = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	//sponza_material_col_b->AddTexture(sponza_column_b);
	//auto sponza_material_col_c = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	//sponza_material_col_c->AddTexture(sponza_column_c);
	//auto sponza_material_flag_pole = getResourceManager()->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	//sponza_material_flag_pole->AddTexture(sponza_flag_pole);

	//SKYBOX
	{
		auto entity = getWorld()->createEntity<Entity>();
		auto meshComponent = entity->createComponent<MeshComponent>();
		auto transform = entity->getTransformComponent();

		meshComponent->setMesh(sky_mesh);
		meshComponent->addMaterial(sky_material);

		transform->setScale(Vector3D(1000.0f, 1000.0f, 1000.0f));
	}

	//TERRAIN
	{
		auto entity = getWorld()->createEntity<Entity>();
		auto terrainComponent = entity->createComponent<TerrainComponent>();
		terrainComponent->setHeightMap(terrain_heighmap);

		auto transform = entity->getTransformComponent();
		transform->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	}

	//FLOOR
	{
		//auto entity = getWorld()->createEntity<Entity>();
		//auto meshComponent = entity->createComponent<MeshComponent>();

		//meshComponent->setMesh(terrain_mesh);
		//meshComponent->addMaterial(terrain_material);
	}

	//HOUSE
	{
		//auto entity = getWorld()->createEntity<Entity>();
		//auto meshComponent = entity->createComponent<MeshComponent>();

		//meshComponent->setMesh(house_mesh);
		//meshComponent->addMaterial(wood_material);
		//meshComponent->addMaterial(wood_material);
		//meshComponent->addMaterial(wood_material);
		//meshComponent->addMaterial(wood_material);
	}

	//SPONZA
	{
		//auto entity = getWorld()->createEntity<Entity>();
		//auto meshComponent = entity->createComponent<MeshComponent>();

		//meshComponent->setMesh(sponza_mesh);
		//meshComponent->addMaterial(sponza_material_arch);
		//meshComponent->addMaterial(sponza_material_brick);
		//meshComponent->addMaterial(sponza_material_col_a);
		//meshComponent->addMaterial(sponza_material_col_b);
		//meshComponent->addMaterial(sponza_material_col_c);
		//meshComponent->addMaterial(sponza_material_flag_pole);
	}

	//BOX
	//for(int x = -7; x < 7; x++)
	//{
	//	for (int y = -7; y < 7; y++)
	//	{
	//		float rand_pos_x = (rand() % 40) - 20.0f;
	//		float rand_pos_z = (rand() % 40) - 20.0f;
	//		
	//		auto entity = getWorld()->createEntity<Entity>();
	//		auto meshComponent = entity->createComponent<MeshComponent>();
	//		auto transform = entity->getTransformComponent();

	//		meshComponent->setMesh(box_mesh);
	//		meshComponent->addMaterial(wood_material);

	//		transform->setPosition(Vector3D(rand_pos_x, 0.0f, rand_pos_z));
	//		transform->setRotation(Vector3D(0.0f, (rand() % 628) / 100.0f, 0.0f));
	//		transform->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	//	}
	//}

	//LIGHT
	{
		m_entity = getWorld()->createEntity<Entity>();
		auto lightComponent = m_entity->createComponent<LightComponent>();

		lightComponent->setColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));

		m_entity->getTransformComponent()->setRotation(Vector3D(-0.707f, 0.707, 0));
	}

	getWorld()->createEntity<Player>();

	getInputSystem()->lockMouseCursor(m_locked);
}