#pragma once
#include "MainWindow.h"
#include "GraphicEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"

class AppWindow : public MainWindow, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	void OnUpdate() override;
	void OnCreate() override;
	void OnDestroy() override;
	void OnFocus() override;
	void OnKillFocus() override;

	void OnKeyUp(int key) override;
	void OnKeyDown(int key) override;

	void OnMouseMove(const Point& mouse_pos) override;
	void OnLeftMouseButtonDown(const Point& delta_mouse_pos) override;
	void OnLeftMouseButtonUp(const Point& delta_mouse_pos) override;
	void OnRightMouseButtonDown(const Point& delta_mouse_pos) override;
	void OnRightMouseButtonUp(const Point& delta_mouse_pos) override;

public:
	void DrawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& texture);

private:
	void Update();
	void UpdateModel();
	void UpdateCamera();
	void UpdateSkyBox();
	void UpdateDeltaTime();

private:
	SwapChainPtr m_swapChain;

	VertexBufferPtr m_vertexBuffer;
	IndexBufferPtr m_indexBuffer;

	ConstantBufferPtr m_constantBuffer;
	ConstantBufferPtr m_skybox_constantBuffer;

	VertexShaderPtr m_vertexShader;

	PixelShaderPtr m_pixelShader;
	PixelShaderPtr m_skyPixelShader;

	TexturePtr m_sky_texture;
	TexturePtr m_wood_texture;

	MeshPtr m_mesh;
	MeshPtr m_sky_mesh;

private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_rot_x;
	float m_rot_y;

	float m_light_rot_y;

	float m_scale_cube;

	float m_forward;
	float m_rightward;

	Matrix4x4 m_world_camera;
	Matrix4x4 m_view_camera;
	Matrix4x4 m_proj_camera;
};

