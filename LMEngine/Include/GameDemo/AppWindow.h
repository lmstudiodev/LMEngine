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
	void OnSize() override;

	void OnKeyUp(int key) override;
	void OnKeyDown(int key) override;

	void OnMouseMove(const Point& mouse_pos) override;
	void OnLeftMouseButtonDown(const Point& delta_mouse_pos) override;
	void OnLeftMouseButtonUp(const Point& delta_mouse_pos) override;
	void OnRightMouseButtonDown(const Point& delta_mouse_pos) override;
	void OnRightMouseButtonUp(const Point& delta_mouse_pos) override;

	void OnGamePadButtonAPressed();
	void OnGamePadButtonYPressed();
	void OnGamePadButtonXPressed();
	void OnGamePadButtonBPressed();
	void OnGamePadLeftStickXChanged(const float value);
	void OnGamePadLeftStickYChanged(const float value);
	void OnGamePadRightStickMoved(const float valueX, const float valueY);
	void OnGamePadLeftThumbPressed(bool value);

public:
	void DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& material_list);

private:
	void Render();
	void Update();
	void UpdateModel(Vector3D position, const std::vector<MaterialPtr>& material_list);
	void UpdateCamera();
	void UpdateSkyBox();
	void UpdateLight();
	void UpdateDeltaTime();

private:
	SwapChainPtr m_swapChain;

	TexturePtr m_sky_texture;
	MeshPtr m_sky_mesh;
	MaterialPtr m_sky_mat;

	TexturePtr m_brick_texture;
	TexturePtr m_brick_normal_texture;
	MeshPtr m_sphere_mesh;
	MaterialPtr m_brick_mat;

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

	float m_time;
	float m_light_radius;

	bool m_play_state;
	bool m_fullscreen_state;

	std::vector<MaterialPtr> m_material_list;

	Vector4D m_lightPosition;
};

