#include "MiniginPCH.h"
#include "GameObject.h"
#include "Component.h"


dae::GameObject::~GameObject()
{
	for (int index = static_cast<int>(m_Components.size()) - 1; index >= 0; index--)
	{
		delete m_Components[index];
		m_Components.pop_back();
	}
}

void dae::GameObject::Update(float deltaSec)
{
	if (m_IsMarkedForDeletion)
		return;

	if (m_IsFirstUpdate)
	{
		for (const auto& object : m_Components)
		{
			object->Startup();
		}
		m_IsFirstUpdate = false;
	}

	for (Component* pElement : m_Components)
	{
		pElement->Update(deltaSec);
	}
}

void dae::GameObject::Render() const
{
	if (m_IsMarkedForDeletion)
		return;

	for (const Component* pElement : m_Components)
	{
		pElement->Render();
	}
}

void dae::GameObject::MarkForDelete()
{
	m_IsMarkedForDeletion = true;
}

bool dae::GameObject::GetIsMarkedForDelete() const
{
	return m_IsMarkedForDeletion;
}



