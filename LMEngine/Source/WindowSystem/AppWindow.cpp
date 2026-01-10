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
m_scale_cube(1.0f),
m_forward(0),
m_rightward(0)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::UpdateMatrix()
{
	Constant cc{};

	Matrix4x4 tempMatrix;

	cc.m_worldMatrix.SetIdentity();

	Matrix4x4 worldCam;
	worldCam.SetIdentity();

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationX(m_rot_x);
	worldCam *= tempMatrix;

	tempMatrix.SetIdentity();
	tempMatrix.SetRotationY(m_rot_y);
	worldCam *= tempMatrix;

	Vector3D newPos = m_world_camera.GetTranslation() + worldCam.GetZDirection() * (m_forward * 0.3f);
	newPos = newPos + worldCam.GetXDirection() * (m_rightward * 0.3f);

	worldCam.SetTranslation(newPos);

	m_world_camera = worldCam;

	worldCam.inverse();

	cc.m_viewMatrix = worldCam;

	RECT rc = this->GetClientWindowRect();

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	cc.m_projectionMatrix.SetPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	m_constantBuffer->Update(GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext(), &cc);
}

void AppWindow::UpdateDeltaTime()
{
	m_old_delta = m_new_delta;
	m_new_delta = GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0.0f;
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

	UpdateMatrix();

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetConstantBuffer(m_vertexShader, m_constantBuffer);
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetConstantBuffer(m_pixelShader, m_constantBuffer);

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetVertexShader(m_vertexShader);
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetPixelShader(m_pixelShader);

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetTexture(m_pixelShader, m_wood_texture);

	//GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetVertexBuffer(m_vertexBuffer);
	//GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetIndexBuffer(m_indexBuffer);
	//GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList(m_indexBuffer->GetSizeIndexList(), 0, 0);

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetVertexBuffer(m_mesh->GetVertexBuffer());
	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->SetIndexBuffer(m_mesh->GetIndexBuffer());

	GraphicEngine::Get()->GetRenderSystem()->GetDeviceContext()->DrawIndexedTriangleList(m_mesh->GetIndexBuffer()->GetSizeIndexList(), 0, 0);

	m_swapChain->Present(true);

	UpdateDeltaTime();
}

void AppWindow::OnCreate()
{
	MainWindow::OnCreate();

	InputSystem::Get()->AddListener(this);

	InputSystem::Get()->ShowMouseCursor(false);

	m_wood_texture = GraphicEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\brick.png");
	m_mesh = GraphicEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\teapot.obj");

	RECT rc = this->GetClientWindowRect();

	auto width = rc.right - rc.left;
	auto height = rc.bottom - rc.top;

	m_swapChain = GraphicEngine::Get()->GetRenderSystem()->CreateSwapChain(this->m_hwnd, UINT(width), UINT(height));

	m_world_camera.SetTranslation(Vector3D(0.0f, 0.0f, -2.0f));

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

	UINT sizeIndexList = ARRAYSIZE(index_list);

	m_indexBuffer = GraphicEngine::Get()->GetRenderSystem()->CreateIndexBuffer(index_list, sizeIndexList);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicEngine::Get()->GetRenderSystem()->CompileVertexShader(L"Resources/Shader/VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertexShader = GraphicEngine::Get()->GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
	m_vertexBuffer = GraphicEngine::Get()->GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(Vertex), sizeList, shader_byte_code, size_shader);

	GraphicEngine::Get()->GetRenderSystem()->ReleaseCompiledShader();

	GraphicEngine::Get()->GetRenderSystem()->CompilePixelShader(L"Resources/Shader/PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixelShader = GraphicEngine::Get()->GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
	GraphicEngine::Get()->GetRenderSystem()->ReleaseCompiledShader();

	Constant cc{};

	m_constantBuffer = GraphicEngine::Get()->GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(Constant));
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
	
	m_rot_x += (mouse_pos.m_axis_y - (height / 2.0f)) * m_delta_time / 5.0f;
	m_rot_y += (mouse_pos.m_axis_x - (width / 2.0f)) * m_delta_time / 5.0f;

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
