#pragma once
#include <Prerequisites.h>
#include <Entity/Component.h>
#include <Math/Rect.h>
#include <Math/Matrix4x4.h>

class CameraComponent : public Component
{
public:
	CameraComponent();
	virtual ~CameraComponent();

public:
	void getViewmatrix(Matrix4x4& matrix);
	void getProjectionMatrix(Matrix4x4& matrix);

	void setNearPlane(f32 nearPlane);
	void setFarPlane(f32 farPlane);
	void setFieldOfView(f32 fov);
	void setCameraType(const CameraType& type);
	void setScreenArea(const Rect& screenArea);

	f32 getNearPlane();
	f32 getFarPlane();
	f32 getFieldOfView();
	CameraType getCameraType();
	Rect getScreenArea();

private:
	void computeProjectionMatrix();

protected:
	virtual void onCreateInternal();

protected:
	f32 m_nearPlane = 0.0f;
	f32 m_farPlane = 100.0f;
	f32 m_fov = 1.3f;

	CameraType m_type = CameraType::Perspective;

	Rect m_screenArea;

	Matrix4x4 m_projectionMatrix;
};

