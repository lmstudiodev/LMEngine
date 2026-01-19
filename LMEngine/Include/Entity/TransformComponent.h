#pragma once
#include <Prerequisites.h>
#include <Entity/Component.h>
#include <Math/Vector3D.h>
#include <Math/Matrix4x4.h>

class TransformComponent : public Component
{
public:
	TransformComponent();
	virtual ~TransformComponent();

public:
	void setPosition(const Vector3D& position);
	void setRotation(const Vector3D& rotation);
	void setScale(const Vector3D& scale);

	Vector3D getPosition();
	Vector3D getRotation();
	Vector3D getScale();

	void getWorldMatrix(Matrix4x4& matrix);

protected:
	void updateWorldMatrix();

protected:
	Vector3D m_position;
	Vector3D m_rotation;
	Vector3D m_scale = Vector3D(1.0f, 1.0f, 1.0f);

	Matrix4x4 m_worldMatrix;
};

