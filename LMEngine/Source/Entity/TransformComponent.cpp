#include <stdafx.h>
#include <Entity/TransformComponent.h>

TransformComponent::TransformComponent()
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::setPosition(const Vector3D& position)
{
	m_position = position;

	updateWorldMatrix();
}

void TransformComponent::setRotation(const Vector3D& rotation)
{
	m_rotation = rotation;

	updateWorldMatrix();
}

void TransformComponent::setScale(const Vector3D& scale)
{
	m_scale = scale;

	updateWorldMatrix();
}

Vector3D TransformComponent::getPosition()
{
	return m_position;
}

Vector3D TransformComponent::getRotation()
{
	return m_rotation;
}

Vector3D TransformComponent::getScale()
{
	return m_scale;
}

void TransformComponent::getWorldMatrix(Matrix4x4& matrix)
{
	matrix.SetMatrix(m_worldMatrix);
}

void TransformComponent::updateWorldMatrix()
{
	Matrix4x4 tmp;

	m_worldMatrix.SetIdentity();

	tmp.SetIdentity();
	tmp.SetScale(m_scale);
	m_worldMatrix *= tmp;

	tmp.SetIdentity();
	tmp.SetRotationX(m_rotation.m_x);
	m_worldMatrix *= tmp;

	tmp.SetIdentity();
	tmp.SetRotationY(m_rotation.m_y);
	m_worldMatrix *= tmp;

	tmp.SetIdentity();
	tmp.SetRotationZ(m_rotation.m_z);
	m_worldMatrix *= tmp;

	tmp.SetIdentity();
	tmp.SetTranslation(m_position);
	m_worldMatrix *= tmp;
}
