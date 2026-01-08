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

private:
	void UpdateMatrix();
	void UpdateDeltaTime();

private:
	SwapChain* m_swapChain;
	VertexBuffer* m_vertexBuffer;
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;
	ConstantBuffer* m_constantBuffer;
	IndexBuffer* m_indexBuffer;

private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_rot_x;
	float m_rot_y;

	float m_scale_cube;

	float m_forward;
	float m_rightward;

	Matrix4x4 m_world_camera;
};

