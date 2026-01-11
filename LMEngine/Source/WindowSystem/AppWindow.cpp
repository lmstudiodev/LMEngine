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
};

AppWindow::AppWindow(): m_swapChain(nullptr), 
m_vertexShader(nullptr), 
m_pixelShader(nullptr), 
m_vertexBuffer(nullptr), 
m_constantBuffer(nullptr),
m_indexBuffer(nullptr),
m_old_delta(0),
m_new_delta(0),
m_delta_time(0),
m_rot_x(0),
m_rot_y(0),
m_light_rot_y(0),
m_scale_cube(1.0f),
m_forward(0),
m_rightward(0)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::Update()
{
	UpdateCamera();
	UpdateModel();
	UpdateSkyBox();
}

void AppWindow::UpdateModel()
{
	Constant cc{};

	Matrix4x4 light_rotation_matrix;

	light_rotation_matrix.SetIdentity();
	light_rotation_matrix.SetRotationY(m_light_rot_y);

	m_light_rot_y += 0.707f * m_delta_time;

	cc.m_worldMatrix.SetIdentity();
	cc.m_viewMatrix = m_view_camera;
	cc.m_projectionMatrix = m_proj_camera;
	cc.m_cameraPosition = m_world_camera.GetTranslation();
	cc.m_lightDirection = light_rotation_matrix.GetZDirection();

	m_constantBuffer->Update(GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext(), &cc);
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

	m_skybox_constantBuffer->Update(GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext(), &cc);
}

void AppWindow::UpdateDeltaTime()
{
	m_old_delta = m_new_delta;
	m_new_delta = GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0.0f;
}

void AppWindow::DrawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& texture)
{
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetConstantBuffer(vs, cb);
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetConstantBuffer(ps, cb);

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetVertexShader(vs);
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetPixelShader(ps);

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetTexture(ps, texture);

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList(mesh->GetIndexBuffer()->GetSizeIndexList(), 0, 0);
}

void AppWindow::OnUpdate()
{
	MainWindow::OnUpdate();

	InputSystem::Get()->Update();
	
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->ClearRenderTarget(this->m_swapChain ,{ 0.0f, 0.3f, 0.4f, 1.0f });

	RECT rc = this->GetClientWindowRect();

	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetViewPortSize(width, height);

	Update();

	GraphicEngine::Get()->GetRenderSystem()->SetRasterizerState(false);
	DrawMesh(m_mesh, m_vertexShader, m_pixelShader, m_constantBuffer, m_wood_texture);
	GraphicEngine::Get()->GetRenderSystem()->SetRasterizerState(true);
	DrawMesh(m_sky_mesh, m_vertexShader, m_skyPixelShader, m_skybox_constantBuffer, m_sky_texture);

	m_swapChain->Present(true);

	UpdateDeltaTime();
}

void AppWindow::OnCreate()
{
	MainWindow::OnCreate();

	InputSystem::Get()->AddListener(this);

	InputSystem::Get()->ShowMouseCursor(false);

	m_wood_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\brick.png");
	m_sky_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\sky.jpg");

	m_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	m_sky_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	RECT rc = this->GetClientWindowRect();

	auto width = rc.right - rc.left;
	auto height = rc.bottom - rc.top;

	m_swapChain = GraphicEngine::Get()->GetRenderSystem()->CreateSwapChain(this->m_hwnd, UINT(width), UINT(height));

	m_world_camera.SetTranslation(Vector3D(0.0f, 0.0f, -1.0f));

	Vector3D position_list[]
	{
		{Vector3D(-0.5f, -0.5f, -0.5f)},
		{Vector3D(-0.5f, 0.5f, -0.5f)},
		{Vector3D(0.5, 0.5f, -0.5f)},
		{Vector3D(0.5, -0.5f, -0.5f)},

		{Vector3D(0.5, -0.5f, 0.5f)},
		{Vector3D(0.5, 0.5f, 0.5f)},
		{Vector3D(-0.5, 0.5f, 0.5f)},
		{Vector3D(-0.5, -0.5f, 0.5f)}
	};

	Vector2D textcoord_list[]
	{
		{Vector2D(0.0f, 0.0f)},
		{Vector2D(0.0f, 1.0f)},
		{Vector2D(1.0f, 0.0f)},
		{Vector2D(1.0f, 1.0f)}
	};

	Vertex vertex_list[]
	{
		//FRONT FACE
		{position_list[0], textcoord_list[1]},
		{position_list[1], textcoord_list[0]},
		{position_list[2], textcoord_list[2]},
		{position_list[3], textcoord_list[3]},
		//BACK FACE
		{position_list[4], textcoord_list[1]},
		{position_list[5], textcoord_list[0]},
		{position_list[6], textcoord_list[2]},
		{position_list[7], textcoord_list[3]},
		//TOP FACE
		{position_list[1], textcoord_list[1]},
		{position_list[6], textcoord_list[0]},
		{position_list[5], textcoord_list[2]},
		{position_list[2], textcoord_list[3]},
		//BOTTOM FACE
		{position_list[7], textcoord_list[1]},
		{position_list[0], textcoord_list[0]},
		{position_list[3], textcoord_list[2]},
		{position_list[4], textcoord_list[3]},
		//RIGHT FACE
		{position_list[3], textcoord_list[1]},
		{position_list[2], textcoord_list[0]},
		{position_list[5], textcoord_list[2]},
		{position_list[4], textcoord_list[3]},
		//LEFT FACE
		{position_list[7], textcoord_list[1]},
		{position_list[6], textcoord_list[0]},
		{position_list[1], textcoord_list[2]},
		{position_list[0], textcoord_list[3]}

	};

	UINT sizeList = ARRAYSIZE(vertex_list);

	unsigned int index_list[]
	{
		//FRONT FACE
		0,1,2,
		2,3,0,
		//BACK FACE
		4,5,6,
		6,7,4,
		//TOP FACE
		8,9,10,
		10,11,8,
		//BOTTOM FACE
		12,13,14,
		14,15,12,
		//RIGHT FACE
		16,17,18,
		18,19,16,
		//LEFT FACE
		20,21,22,
		22,23,20
	};

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicEngine::Get()->GetRenderSystem()->CompileVertexShader(L"Resources/Shader/VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertexShader = GraphicEngine::Get()->GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
	GraphicEngine::Get()->GetRenderSystem()->ReleaseCompiledShader();

	GraphicEngine::Get()->GetRenderSystem()->CompilePixelShader(L"Resources/Shader/PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixelShader = GraphicEngine::Get()->GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
	GraphicEngine::Get()->GetRenderSystem()->ReleaseCompiledShader();

	GraphicEngine::Get()->GetRenderSystem()->CompilePixelShader(L"Resources/Shader/SkyBoxShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_skyPixelShader = GraphicEngine::Get()->GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
	GraphicEngine::Get()->GetRenderSystem()->ReleaseCompiledShader();

	Constant cc{};

	m_constantBuffer = GraphicEngine::Get()->GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(Constant));
	m_skybox_constantBuffer = GraphicEngine::Get()->GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(Constant));
}

void AppWindow::OnFocus()
{
	InputSystem::Get()->AddListener(this);
}

void AppWindow::OnKillFocus()
{
	InputSystem::Get()->RemoveListener(this);
}

void AppWindow::OnDestroy()
{
	InputSystem::Get()->ShowMouseCursor(true);
	
	MainWindow::OnDestroy();
}

void AppWindow::OnKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
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
}

void AppWindow::OnMouseMove(const Point& mouse_pos)
{
	RECT rc = this->GetClientWindowRect();

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	
	m_rot_x += (mouse_pos.m_axis_y - (height / 2.0f)) * m_delta_time;
	m_rot_y += (mouse_pos.m_axis_x - (width / 2.0f)) * m_delta_time;

	InputSystem::Get()->SetCursorPosition(Point( (width / 2.0f), (height / 2.0f) ));
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
