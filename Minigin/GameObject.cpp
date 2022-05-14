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

void dae::GameObject::Startup()
{
	for (Component* pElement : m_Components)
	{
		pElement->Startup();
	}
}

void dae::GameObject::Update()
{
	for (Component* pElement : m_Components)
	{
		pElement->Update();
	}
}

void dae::GameObject::Render() const
{
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



