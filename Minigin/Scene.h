#pragma once
#include "SceneManager.h"
#include <map>

namespace dae
{
	class GameObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Add(const std::shared_ptr<GameObject>& object);
		void Remove(const GameObject* object);
		void Remove(std::shared_ptr<GameObject>& object);

		void Update(float deltaSec);
		void Render() const;

		void DeleteRemovedObjects();
		void DeleteRemovedObjectsFromVector(std::vector<std::shared_ptr<GameObject>>& vectorToRemoveFrom);
		void AddObjects();

		std::string GetName() const;

	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < std::shared_ptr<GameObject>> m_Objects{};
		std::map<unsigned int, std::vector<std::shared_ptr<GameObject>>> m_ObjectsToAdd{};


		static unsigned int m_IdCounter;
	};

}
