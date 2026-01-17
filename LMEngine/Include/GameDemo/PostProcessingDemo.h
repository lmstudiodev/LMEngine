#pragma once
#include <MainWindow.h>
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

class PostProcessingDemo : public MainWindow, public InputListener
{
public:
	PostProcessingDemo();
	~PostProcessingDemo();

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
	void UpdateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& material_list);
	void UpdateCamera();
	void UpdateThirdPersonCamera();
	void UpdateSkyBox();
	void UpdateSpaceShip();
	void UpdateLight();
	void UpdateDeltaTime();

private:
	SwapChainPtr m_swapChain;

	TexturePtr m_spaceship_texture;
	MeshPtr m_spaceship_mesh;
	MaterialPtr m_spaceship_mat;

	TexturePtr m_asteroid_texture;
	MeshPtr m_asteroid_mesh;
	MaterialPtr m_asteroid_mat;

	TexturePtr m_sky_texture;
	MeshPtr m_sky_mesh;
	MaterialPtr m_sky_mat;

	MaterialPtr m_base_mat;

	TexturePtr m_render_target;
	TexturePtr m_depth_stencil;

	MeshPtr m_quad_mesh;
	MaterialPtr m_post_process_material;

private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	Vector3D m_camera_rotation;
	Vector3D m_camera_position;
	Vector3D m_camera_current_rotation;

	Vector3D m_spaceship_rotation;
	Vector3D m_spaceship_position;
	Vector3D m_spaceship_current_rotation;
	Vector3D m_spaceship_current_position;

	float m_spaceship_speed;
	float m_spaceship_rotation_speed;

	float m_camera_current_distance;
	float m_camera_distance;

	float m_delta_mouse_x;
	float m_delta_mouse_y;

	float m_light_rot_y;

	float m_forward;
	float m_rightward;

	float m_time;

	float m_distortion_level;

	bool m_play_state;
	bool m_fullscreen_state;
	bool cull;
	bool m_anim;
	bool m_turbo_mode;
	bool m_use_gamePad;

	Matrix4x4 m_world_camera;
	Matrix4x4 m_view_camera;
	Matrix4x4 m_proj_camera;
	Matrix4x4 light_rotation_matrix;

	std::vector<MaterialPtr> m_material_list;

	Vector4D m_lightPosition;

	Vector3D m_asteroid_pos_list[200];
	Vector3D m_asteroid_rot_list[200];
	Vector3D m_asteroid_scale_list[200];

	unsigned int m_num_of_asteroids = 200;
};



