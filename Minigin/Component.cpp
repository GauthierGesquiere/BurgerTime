#include "MiniginPCH.h"
#include "Component.h"
#include "GameObject.h"


void dae::Component::Startup()
{
}

void dae::Component::Update(float /*deltaSec*/)
{
}

void dae::Component::Render() const
{
}

void dae::Component::SetOwner(dae::GameObject* newOwner)
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->RemoveComponent(this, true);
	}
	m_pOwner = newOwner;
}

dae::GameObject* dae::Component::GetOwner() const
{
	return m_pOwner;
}
