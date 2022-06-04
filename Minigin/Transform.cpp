#include "MiniginPCH.h"
#include "Transform.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::MoveWith(const glm::vec3 pos)
{
	m_Position.x += pos.x;
	m_Position.y += pos.y;
	m_Position.z += pos.z;
}
