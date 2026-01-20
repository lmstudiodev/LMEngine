#include <stdafx.h>
#include <Entity/CameraComponent.h>
#include <Entity/TransformComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Core/GraphicEngine.h>

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
	m_entity->getWorld()->getGame()->getGraphicEngine()->removeComponent(this);
}

void CameraComponent::getViewmatrix(Matrix4x4& matrix)
{
	m_entity->getTransformComponent()->getWorldMatrix(matrix);
	matrix.inverse();
}

void CameraComponent::getProjectionMatrix(Matrix4x4& matrix)
{
	matrix = m_projectionMatrix;
}

void CameraComponent::setNearPlane(f32 nearPlane)
{
	m_nearPlane = nearPlane;

	computeProjectionMatrix();
}

void CameraComponent::setFarPlane(f32 farPlane)
{
	m_farPlane = farPlane;

	computeProjectionMatrix();
}

void CameraComponent::setFieldOfView(f32 fov)
{
	m_fov = fov;

	computeProjectionMatrix();
}

void CameraComponent::setCameraType(const CameraType& type)
{
	m_type = type;

	computeProjectionMatrix();
}

void CameraComponent::setScreenArea(const Rect& screenArea)
{
	m_screenArea = screenArea;

	computeProjectionMatrix();
}

f32 CameraComponent::getNearPlane()
{
	return m_nearPlane;
}

f32 CameraComponent::getFarPlane()
{
	return m_farPlane;
}

f32 CameraComponent::getFieldOfView()
{
	return m_fov;
}

CameraType CameraComponent::getCameraType()
{
	return m_type;
}

Rect CameraComponent::getScreenArea()
{
	return m_screenArea;
}

void CameraComponent::computeProjectionMatrix()
{
	if (m_type == CameraType::Perspective)
	{
		m_projectionMatrix.SetPerspectiveFovLH(m_fov, (f32)m_screenArea.width / (f32)m_screenArea.height, m_nearPlane, m_farPlane);
	}
	else if (m_type == CameraType::Orthogonal)
	{
		m_projectionMatrix.SetOrthoLH((f32)m_screenArea.width, (f32)m_screenArea.height, m_nearPlane, m_farPlane);
	}
}

void CameraComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicEngine()->addComponent(this);
}
