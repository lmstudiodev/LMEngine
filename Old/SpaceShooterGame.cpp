#include <stdafx.h>
#include "SpaceShooterGame.h"
#include "Vertex.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "MathUtils.h"

constexpr auto PI = 3.14159265359;

__declspec(align(16))
struct Constant
{
	Matrix4x4 m_worldMatrix;
	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projectionMatrix;
	Vector4D m_lightDirection;
	Vector4D m_cameraPosition;
	Vector4D m_lightPosition = Vector4D(0.0f, 1.0f, 0.0f, 0.0f);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
};

SpaceShooterGame::SpaceShooterGame() : m_swapChain(nullptr),
m_old_delta(0),
m_new_delta(0),
m_delta_time(0),
m_delta_mouse_x(0),
m_delta_mouse_y(0),
m_light_rot_y(0),
m_forward(0),
m_rightward(0),
m_play_state(false),
m_fullscreen_state(false),
m_time(0),
cull(true),
m_anim(false),
m_turbo_mode(false),
m_camera_rotation(Vector3D(0, 0, 0)),
m_camera_position(Vector3D(0, 0, 0)),
m_camera_current_rotation(Vector3D(0, 0, 0)),
m_spaceship_position(Vector3D(0, 0, 0)),
m_spaceship_rotation(Vector3D(0, 0, 0)),
m_spaceship_current_position(Vector3D(0, 0, 0)),
m_spaceship_current_rotation(Vector3D(0, 0, 0)),
m_camera_distance(14.0f),
m_camera_current_distance(0),
m_spaceship_speed(125.0f),
m_spaceship_rotation_speed(20.0f)
{
}

SpaceShooterGame::~SpaceShooterGame()
{
}

void SpaceShooterGame::DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& material_list)
{
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

	for (size_t m = 0; m < mesh->GetNumMaterialSlots(); m++)
	{
		if (m >= material_list.size())
			break;

		MaterialSlot mat = mesh->GetMaterialSlot(m);

		GraphicEngine::Get()->SetMaterial(material_list[m]);

		GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
	}
}

void SpaceShooterGame::Render()
{
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->ClearRenderTarget(this->m_swapChain, { 0.0f, 0.3f, 0.4f, 1.0f });

	RECT rc = this->GetClientWindowRect();

	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetViewPortSize(width, height);

	//SPACE SHIP
	m_material_list.clear();
	m_material_list.push_back(m_spaceship_mat);
	UpdateModel(m_spaceship_current_position, m_spaceship_current_rotation, Vector3D(1.0f, 1.0f, 1.0f), m_material_list);
	DrawMesh(m_spaceship_mesh, m_material_list);

	//ASTEROIDS
	m_material_list.clear();
	m_material_list.push_back(m_asteroid_mat);

	for (unsigned int i = 0; i < m_num_of_asteroids; i++)
	{
		UpdateModel(m_asteroid_pos_list[i], m_asteroid_rot_list[i], m_asteroid_scale_list[i], m_material_list);
		DrawMesh(m_asteroid_mesh, m_material_list);
	}

	//DRAW SKY BOX
	m_material_list.clear();
	m_material_list.push_back(m_sky_mat);
	DrawMesh(m_sky_mesh, m_material_list);

	m_swapChain->Present(true);

	UpdateDeltaTime();
}

void SpaceShooterGame::Update()
{
	UpdateSpaceShip();
	UpdateThirdPersonCamera();
	UpdateLight();
	UpdateSkyBox();
}

void SpaceShooterGame::UpdateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& material_list)
{
	Constant cc{};

	Matrix4x4 temp;

	cc.m_worldMatrix.SetIdentity();

	temp.SetIdentity();
	temp.SetScale(scale);
	cc.m_worldMatrix *= temp;

	temp.SetIdentity();
	temp.SetRotationX(rotation.m_x);
	cc.m_worldMatrix *= temp;

	temp.SetIdentity();
	temp.SetRotationY(rotation.m_y);
	cc.m_worldMatrix *= temp;

	temp.SetIdentity();
	temp.SetRotationZ(rotation.m_z);
	cc.m_worldMatrix *= temp;

	temp.SetIdentity();
	temp.SetTranslation(position);
	cc.m_worldMatrix *= temp;

	cc.m_viewMatrix = m_view_camera;
	cc.m_projectionMatrix = m_proj_camera;
	cc.m_cameraPosition = m_world_camera.GetTranslation();
	cc.m_lightPosition = m_lightPosition;
	cc.m_light_radius = 0.0f;
	cc.m_lightDirection = light_rotation_matrix.GetZDirection();
	cc.m_time = m_time;

	for (size_t m = 0; m < material_list.size(); m++)
	{
		material_list[m]->SetData(&cc, sizeof(Constant));
	}
}

void SpaceShooterGame::UpdateCamera()
{
	Matrix4x4 worldCam;
	Matrix4x4 tempMatrix;
	worldCam.SetIdentity();

	m_camera_rotation.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_camera_rotation.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationX(m_camera_rotation.m_x);
	worldCam *= tempMatrix;

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationY(m_camera_rotation.m_y);
	worldCam *= tempMatrix;

	Vector3D newPos = m_world_camera.GetTranslation() + worldCam.GetZDirection() * (m_forward * 0.1f);
	newPos = newPos + worldCam.GetXDirection() * (m_rightward * 0.05f);

	worldCam.SetTranslation(newPos);

	m_world_camera = worldCam;

	worldCam.inverse();

	m_view_camera = worldCam;

	RECT rc = this->GetClientWindowRect();

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	m_proj_camera.SetPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void SpaceShooterGame::UpdateThirdPersonCamera()
{
	Matrix4x4 worldCam;
	Matrix4x4 tempMatrix;
	worldCam.SetIdentity();

	m_camera_rotation.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_camera_rotation.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	float maxRotationAngle = 90.0f * (PI / 180);

	if (m_camera_rotation.m_x >= maxRotationAngle)
		m_camera_rotation.m_x = maxRotationAngle;
	else if (m_camera_rotation.m_x <= -maxRotationAngle)
		m_camera_rotation.m_x = -maxRotationAngle;

	m_camera_current_rotation = Vector3D::Lerp(m_camera_current_rotation, m_camera_rotation, 3.0f * m_delta_time);

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationX(m_camera_current_rotation.m_x);
	worldCam *= tempMatrix;

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationY(m_camera_current_rotation.m_y);
	worldCam *= tempMatrix;

	if (m_forward)
	{
		if (m_turbo_mode)
		{
			m_camera_distance = m_forward > 0 ? 25.0f : 5.0f;
		}
		else
		{
			m_camera_distance = m_forward > 0 ? 16.0f : 9.0f;
		}
	}
	else
	{
		m_camera_distance = 14.0f;
	}

	m_camera_current_distance = MathUtils::Lerp(m_camera_current_distance, m_camera_distance, 2.0f * m_delta_time);

	m_camera_position = m_spaceship_current_position;

	Vector3D newPos = m_camera_position + worldCam.GetZDirection() * (-m_camera_current_distance);
	newPos = newPos + worldCam.GetYDirection() * (5.0f);

	worldCam.SetTranslation(newPos);

	m_world_camera = worldCam;

	worldCam.inverse();

	m_view_camera = worldCam;

	RECT rc = this->GetClientWindowRect();

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	m_proj_camera.SetPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 5000.0f);
}

void SpaceShooterGame::UpdateSkyBox()
{
	Constant cc{};

	cc.m_worldMatrix.SetIdentity();
	cc.m_worldMatrix.SetScale(Vector3D(4000.0f, 4000.0f, 4000.0f));
	cc.m_worldMatrix.SetTranslation(m_world_camera.GetTranslation());
	cc.m_viewMatrix = m_view_camera;
	cc.m_projectionMatrix = m_proj_camera;

	m_sky_mat->SetData(&cc, sizeof(Constant));
}

void SpaceShooterGame::UpdateSpaceShip()
{
	Matrix4x4 worldModel;
	Matrix4x4 tempMatrix;
	worldModel.SetIdentity();

	m_spaceship_rotation.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_spaceship_rotation.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

	float maxRotationAngle = 90.0f * (PI / 180);

	if (m_spaceship_rotation.m_x >= maxRotationAngle)
		m_spaceship_rotation.m_x = maxRotationAngle;
	else if (m_spaceship_rotation.m_x <= -maxRotationAngle)
		m_spaceship_rotation.m_x = -maxRotationAngle;

	m_spaceship_current_rotation = Vector3D::Lerp(m_spaceship_current_rotation, m_spaceship_rotation, 5.0f * m_delta_time);

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationX(m_spaceship_current_rotation.m_x);
	worldModel *= tempMatrix;

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationY(m_spaceship_current_rotation.m_y);
	worldModel *= tempMatrix;

	m_spaceship_speed = 125.0f;

	if (m_turbo_mode)
	{
		m_spaceship_speed = 305.0f;
	}

	m_spaceship_position = m_spaceship_position + worldModel.GetZDirection() * (m_forward) * m_spaceship_speed * m_delta_time;

	m_spaceship_current_position = Vector3D::Lerp(m_spaceship_current_position, m_spaceship_position, 3.0f * m_delta_time);
}

void SpaceShooterGame::UpdateLight()
{
	Matrix4x4 tempMatrix;
	
	light_rotation_matrix.SetIdentity();

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationX(-0.707f);
	light_rotation_matrix *= tempMatrix;

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationY(0.707f);
	light_rotation_matrix *= tempMatrix;
}

void SpaceShooterGame::UpdateDeltaTime()
{
	m_old_delta = m_new_delta;
	m_new_delta = GetTickCount64();
	m_delta_time = 1.0f / 60.0f;

	m_time += m_delta_time;
}

void SpaceShooterGame::OnUpdate()
{
	MainWindow::OnUpdate();

	InputSystem::Get()->Update();
	this->Update();
	this->Render();

	m_delta_mouse_x = 0.0f;
	m_delta_mouse_y = 0.0f;
}

void SpaceShooterGame::OnCreate()
{
	MainWindow::OnCreate();

	InputSystem::Get()->AddListener(this);

	RECT rc = this->GetClientWindowRect();

	auto width = rc.right - rc.left;
	auto height = rc.bottom - rc.top;

	m_swapChain = GraphicEngine::Get()->GetRenderSystem()->CreateSwapChain(this->m_hwnd, UINT(width), UINT(height));

	srand(time(NULL));

	for (unsigned int i = 0; i < m_num_of_asteroids; i++)
	{
		float rand_pos_x = rand() % 4000 + (-2000);
		float rand_pos_y = rand() % 4000 + (-2000);
		float rand_pos_z = rand() % 4000 + (-2000);

		float rand_rot_x = (rand() % 628) / 100.0f;
		float rand_rot_y = (rand() % 628) / 100.0f;
		float rand_rot_z = (rand() % 628) / 100.0f;

		float rand_scale = rand() % 20 + (6);

		m_asteroid_pos_list[i] = Vector3D(rand_pos_x, rand_pos_y, rand_pos_z);
		m_asteroid_rot_list[i] = Vector3D(rand_rot_x, rand_rot_y, rand_rot_z);
		m_asteroid_scale_list[i] = Vector3D(rand_scale, rand_scale, rand_scale);
	}

	m_sky_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	m_spaceship_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\spaceship.jpg");
	m_asteroid_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\asteroid.jpg");

	m_sky_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_spaceship_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\spaceship.obj");
	m_asteroid_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\asteroid.obj");

	m_base_mat = GraphicEngine::Get()->CreateMaterial(L"Resources/Shader/DirectionalLightVertexShader.hlsl", L"Resources/Shader/DirectionalLightPixelShader.hlsl");
	m_base_mat->SetCullMode(CULL_MODE_BACK);

	m_sky_mat = GraphicEngine::Get()->CreateMaterial(L"Resources/Shader/SkyBoxVertexShader.hlsl", L"Resources/Shader/SkyBoxPixelShader.hlsl");
	m_sky_mat->AddTexture(m_sky_texture);
	m_sky_mat->SetCullMode(CULL_MODE_FRONT);

	m_spaceship_mat = GraphicEngine::Get()->CreateMaterial(m_base_mat);
	m_spaceship_mat->AddTexture(m_spaceship_texture);
	m_spaceship_mat->SetCullMode(CULL_MODE_BACK);

	m_asteroid_mat = GraphicEngine::Get()->CreateMaterial(m_base_mat);
	m_asteroid_mat->AddTexture(m_asteroid_texture);
	m_asteroid_mat->SetCullMode(CULL_MODE_BACK);

	m_world_camera.SetTranslation(Vector3D(0.0f, 1.0f, -2.0f));

	m_material_list.reserve(32);
}

void SpaceShooterGame::OnFocus()
{
	InputSystem::Get()->AddListener(this);
}

void SpaceShooterGame::OnKillFocus()
{
	InputSystem::Get()->RemoveListener(this);
}

void SpaceShooterGame::OnSize()
{
	MainWindow::OnSize();

	RECT rc = this->GetClientWindowRect();

	auto width = rc.right - rc.left;
	auto height = rc.bottom - rc.top;

	m_swapChain->Resize(width, height);

	this->Update();
	this->Render();
}

void SpaceShooterGame::OnDestroy()
{
	MainWindow::OnDestroy();
	m_swapChain->SetFullscreen(false, 1, 1);
}

void SpaceShooterGame::OnKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == VK_ESCAPE)
	{
		m_play_state = !m_play_state;
		InputSystem::Get()->ShowMouseCursor(!m_play_state);
	}
	else if (key == VK_SHIFT)
	{
		if (m_play_state)
			m_turbo_mode = false;
	}
	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;

		RECT screenSize = GetScreenSize();

		m_swapChain->SetFullscreen(m_fullscreen_state, screenSize.right, screenSize.bottom);
	}
}

void SpaceShooterGame::OnKeyDown(int key)
{
	if (!m_play_state)
		return;
	
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
	}
	else if (key == VK_SHIFT)
	{
		m_turbo_mode = true;
	}
}

void SpaceShooterGame::OnMouseMove(const Point& mouse_pos)
{
	if(!m_play_state)
		return;
	
	RECT rc = this->GetClientWindowRect();

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	m_delta_mouse_x = (int)(mouse_pos.m_axis_x - (int)(rc.left + (width / 2.0f)));
	m_delta_mouse_y = (int)(mouse_pos.m_axis_y - (int)(rc.top + (height / 2.0f)));

	InputSystem::Get()->SetCursorPosition(Point(rc.left + (int)(width / 2.0f), rc.top + (int)(height / 2.0f) ));
}

void SpaceShooterGame::OnLeftMouseButtonDown(const Point& delta_mouse_pos)
{
	if (!m_play_state)
		return;
}

void SpaceShooterGame::OnLeftMouseButtonUp(const Point& delta_mouse_pos)
{
	if (!m_play_state)
		return;
}

void SpaceShooterGame::OnRightMouseButtonDown(const Point& delta_mouse_pos)
{
	if (!m_play_state)
		return;
}

void SpaceShooterGame::OnRightMouseButtonUp(const Point& delta_mouse_pos)
{
	if (!m_play_state)
		return;
}

void SpaceShooterGame::OnGamePadButtonAPressed()
{
	if (!m_play_state)
		return;

	std::cout << "A BUTTON PRESSED" << "\n";
}

void SpaceShooterGame::OnGamePadButtonYPressed()
{
	if (!m_play_state)
		return;

	std::cout << "Y BUTTON PRESSED" << "\n";
}

void SpaceShooterGame::OnGamePadButtonXPressed()
{
	if (!m_play_state)
		return;

	std::cout << "X BUTTON PRESSED" << "\n";
}

void SpaceShooterGame::OnGamePadButtonBPressed()
{
	std::cout << "B BUTTON PRESSED" << "\n";
}

void SpaceShooterGame::OnGamePadLeftStickXChanged(const float value)
{
	if (!m_play_state)
		return;

	m_rightward = value;
}

void SpaceShooterGame::OnGamePadLeftStickYChanged(const float value)
{
	if (!m_play_state)
		return;

	m_forward = value;
}

void SpaceShooterGame::OnGamePadRightStickMoved(const float valueX, const float valueY)
{
	if (!m_play_state)
		return;

	if (valueX != 0 || valueY != 0)
	{
		m_delta_mouse_x = (int)(valueX * m_spaceship_rotation_speed);
		m_delta_mouse_y = (int)(valueY * m_spaceship_rotation_speed);
	}
}

void SpaceShooterGame::OnGamePadLeftThumbPressed(bool value)
{
	if (!m_play_state)
		return;

	m_turbo_mode = value;
}

