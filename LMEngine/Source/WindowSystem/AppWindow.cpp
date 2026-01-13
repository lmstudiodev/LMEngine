#include "stdafx.h"
#include "AppWindow.h"
#include "Vertex.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"

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

AppWindow::AppWindow(): m_swapChain(nullptr),
m_old_delta(0),
m_new_delta(0),
m_delta_time(0),
m_rot_x(0),
m_rot_y(0),
m_light_rot_y(0),
m_scale_cube(1.0f),
m_forward(0),
m_rightward(0),
m_play_state(false),
m_fullscreen_state(false),
m_time(0),
m_light_radius(500.0f)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& material_list)
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

void AppWindow::Render()
{
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->ClearRenderTarget(this->m_swapChain, { 0.0f, 0.3f, 0.4f, 1.0f });

	RECT rc = this->GetClientWindowRect();

	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetViewPortSize(width, height);

	Update();

	m_material_list.clear();
	m_material_list.push_back(m_terrain_mat);
	UpdateModel(Vector3D(0, 0, 0), m_material_list);
	DrawMesh(m_terrain_mesh, m_material_list);

	m_material_list.clear();
	m_material_list.push_back(m_barrel_mat);
	m_material_list.push_back(m_brick_mat);
	m_material_list.push_back(m_windows_mat);
	m_material_list.push_back(m_woods_mat);

	UpdateModel(Vector3D(0, 0, 10), m_material_list);
	DrawMesh(m_house_mesh, m_material_list);

	m_material_list.clear();
	m_material_list.push_back(m_sky_mat);
	DrawMesh(m_sky_mesh, m_material_list);

	m_swapChain->Present(true);

	UpdateDeltaTime();
}

void AppWindow::Update()
{
	UpdateCamera();
	UpdateLight();
	UpdateSkyBox();
}

void AppWindow::UpdateModel(Vector3D position, const std::vector<MaterialPtr>& material_list)
{
	Constant cc{};

	Matrix4x4 light_rotation_matrix;

	light_rotation_matrix.SetIdentity();
	light_rotation_matrix.SetRotationY(m_light_rot_y);

	cc.m_worldMatrix.SetIdentity();
	cc.m_worldMatrix.SetTranslation(position);
	cc.m_viewMatrix = m_view_camera;
	cc.m_projectionMatrix = m_proj_camera;
	cc.m_cameraPosition = m_world_camera.GetTranslation();
	cc.m_lightPosition = m_lightPosition;
	cc.m_light_radius = m_light_radius;
	cc.m_lightDirection = light_rotation_matrix.GetZDirection();
	cc.m_time = m_time;

	for (size_t m = 0; m < material_list.size(); m++)
	{
		material_list[m]->SetData(&cc, sizeof(Constant));
	}
}

void AppWindow::UpdateCamera()
{
	Matrix4x4 worldCam;
	Matrix4x4 tempMatrix;
	worldCam.SetIdentity();

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationX(m_rot_x);
	worldCam *= tempMatrix;

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationY(m_rot_y);
	worldCam *= tempMatrix;

	Vector3D newPos = m_world_camera.GetTranslation() + worldCam.GetZDirection() * (m_forward * 0.05f);
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

void AppWindow::UpdateSkyBox()
{
	Constant cc{};

	cc.m_worldMatrix.SetIdentity();
	cc.m_worldMatrix.SetScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_worldMatrix.SetTranslation(m_world_camera.GetTranslation());
	cc.m_viewMatrix = m_view_camera;
	cc.m_projectionMatrix = m_proj_camera;

	m_sky_mat->SetData(&cc, sizeof(Constant));
}

void AppWindow::UpdateLight()
{
	m_light_rot_y += 1.57f * m_delta_time;

	float distance_from_origin = 1.0f;

	//m_lightPosition = Vector4D(cos(m_light_rot_y) * distance_from_origin, 1.0f, sin(m_light_rot_y) * distance_from_origin, 1.0f);
	m_lightPosition = Vector4D(180, 140, 70, 1.0f);
}

void AppWindow::UpdateDeltaTime()
{
	m_old_delta = m_new_delta;
	m_new_delta = GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0.0f;

	m_time += m_delta_time;
}

void AppWindow::OnUpdate()
{
	MainWindow::OnUpdate();

	InputSystem::Get()->Update();

	this->Render();
}

void AppWindow::OnCreate()
{
	MainWindow::OnCreate();

	InputSystem::Get()->AddListener(this);

	m_sky_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\sky.jpg");
	m_terrain_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\sand.jpg");

	m_barrel_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\barrel.jpg");
	m_brick_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\house_brick.jpg");
	m_windows_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\house_windows.jpg");
	m_woods_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\house_wood.jpg");

	m_sky_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_terrain_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\terrain.obj");
	m_house_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\house.obj"); //LowPolyPit

	RECT rc = this->GetClientWindowRect();

	auto width = rc.right - rc.left;
	auto height = rc.bottom - rc.top;

	m_swapChain = GraphicEngine::Get()->GetRenderSystem()->CreateSwapChain(this->m_hwnd, UINT(width), UINT(height));

	m_sky_mat = GraphicEngine::Get()->CreateMaterial(L"Resources/Shader/PointLightVertexShader.hlsl", L"Resources/Shader/SkyBoxShader.hlsl");
	m_sky_mat->AddTexture(m_sky_texture);
	m_sky_mat->SetCullMode(CULL_MODE_FRONT);

	m_terrain_mat = GraphicEngine::Get()->CreateMaterial(L"Resources/Shader/PointLightVertexShader.hlsl", L"Resources/Shader/PointLightPixelShader.hlsl");
	m_terrain_mat->AddTexture(m_terrain_texture);
	m_terrain_mat->SetCullMode(CULL_MODE_BACK);

	m_barrel_mat = GraphicEngine::Get()->CreateMaterial(L"Resources/Shader/PointLightVertexShader.hlsl", L"Resources/Shader/PointLightPixelShader.hlsl");
	m_barrel_mat->AddTexture(m_barrel_texture);
	m_barrel_mat->SetCullMode(CULL_MODE_BACK);

	m_brick_mat = GraphicEngine::Get()->CreateMaterial(L"Resources/Shader/PointLightVertexShader.hlsl", L"Resources/Shader/PointLightPixelShader.hlsl");
	m_brick_mat->AddTexture(m_brick_texture);
	m_brick_mat->SetCullMode(CULL_MODE_BACK);

	m_windows_mat = GraphicEngine::Get()->CreateMaterial(L"Resources/Shader/PointLightVertexShader.hlsl", L"Resources/Shader/PointLightPixelShader.hlsl");
	m_windows_mat->AddTexture(m_windows_texture);
	m_windows_mat->SetCullMode(CULL_MODE_BACK);

	m_woods_mat = GraphicEngine::Get()->CreateMaterial(L"Resources/Shader/PointLightVertexShader.hlsl", L"Resources/Shader/PointLightPixelShader.hlsl");
	m_woods_mat->AddTexture(m_woods_texture);
	m_woods_mat->SetCullMode(CULL_MODE_BACK);

	m_world_camera.SetTranslation(Vector3D(0.0f, 1.0f, -2.0f));

	m_material_list.reserve(32);
}

void AppWindow::OnFocus()
{
	InputSystem::Get()->AddListener(this);
}

void AppWindow::OnKillFocus()
{
	InputSystem::Get()->RemoveListener(this);
}

void AppWindow::OnSize()
{
	MainWindow::OnSize();

	RECT rc = this->GetClientWindowRect();

	auto width = rc.right - rc.left;
	auto height = rc.bottom - rc.top;

	m_swapChain->Resize(width, height);

	this->Render();
}

void AppWindow::OnDestroy()
{	
	MainWindow::OnDestroy();
	m_swapChain->SetFullscreen(false, 1, 1);
}

void AppWindow::OnKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'G')
	{
		m_play_state = (m_play_state) ? false : true;
		InputSystem::Get()->ShowMouseCursor(!m_play_state);
	}
	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;

		RECT screenSize = GetScreenSize();

		m_swapChain->SetFullscreen(m_fullscreen_state, screenSize.right, screenSize.bottom);
	}
}

void AppWindow::OnKeyDown(int key)
{
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
	else if (key == 'O')
	{
		m_light_radius -= 1.0f * m_delta_time;
	}
	else if (key == 'P')
	{
		m_light_radius += 1.0f * m_delta_time;
	}
}

void AppWindow::OnMouseMove(const Point& mouse_pos)
{
	if(!m_play_state)
		return;
	
	RECT rc = this->GetClientWindowRect();

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	
	m_rot_x += (mouse_pos.m_axis_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_axis_x - (width / 2.0f)) * m_delta_time * 0.1f;

	InputSystem::Get()->SetCursorPosition(Point( (int)(width / 2.0f), (int)(height / 2.0f) ));
}

void AppWindow::OnLeftMouseButtonDown(const Point& delta_mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::OnLeftMouseButtonUp(const Point& delta_mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::OnRightMouseButtonDown(const Point& delta_mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::OnRightMouseButtonUp(const Point& delta_mouse_pos)
{
	m_scale_cube = 1.0f;
}
