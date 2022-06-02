#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name{ name }
{}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>&object)
{
	/*
	 * C++ map find() function is used to find an element with the given key
	 * value k. If it finds the element then it returns an iterator pointing
	 * to the element. Otherwise, it returns an iterator pointing to the end
	 * of the map, i.e., map::end().
	 */
	const auto& findResult = m_ObjectsToAdd.find(0);
	if (findResult == m_ObjectsToAdd.end())
	{
		m_ObjectsToAdd[0] = std::vector<std::shared_ptr<GameObject>>{};
	}
	m_ObjectsToAdd[0].push_back(object);
}

void Scene::Remove(const GameObject * object)
{
	for (auto& gameObject : m_Objects)
	{
		/*
		 * A pointer to the managed object, or a null pointer.
		 * pointer is a member type, defined as the pointer type that points
		 * to the type of object managed.
		 */
		if (gameObject.get() == object)
		{
			Remove(gameObject);
			return;
		}
	}
}

void Scene::Remove(std::shared_ptr<GameObject>&object)
{
	object->MarkForDelete();
}

void Scene::Update(float deltaSec)
{
	for (auto& object : m_Objects)
	{
		object->Update(deltaSec);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void Scene::DeleteRemovedObjects()
{
	DeleteRemovedObjectsFromVector(m_Objects);
}

void Scene::DeleteRemovedObjectsFromVector(std::vector<std::shared_ptr<GameObject>>&vectorToRemoveFrom)
{
	for (int index = 0; index < static_cast<int>(vectorToRemoveFrom.size()); index++)
	{
		if (vectorToRemoveFrom[index]->GetIsMarkedForDelete())
		{
			vectorToRemoveFrom[index] = vectorToRemoveFrom[vectorToRemoveFrom.size() - 1];
			vectorToRemoveFrom.pop_back();
			index--;
		}
	}
}

void Scene::AddObjects()
{
	for (auto& pair : m_ObjectsToAdd)
	{
		for (auto& gameObject : pair.second)
		{
			m_Objects.push_back(gameObject);
		}
	}
	m_ObjectsToAdd.clear();
}

std::string dae::Scene::GetName() const
{
	return m_Name;
}

