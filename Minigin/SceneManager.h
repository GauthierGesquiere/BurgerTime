#pragma once
#include "Singleton.h"
#include <string>
#include <vector>
#include <memory>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		std::shared_ptr<Scene> GetScene(const std::string& name) const;
		std::shared_ptr<Scene> GetActiveScene() const;

		void SetSceneAsActive(const std::string& name);
		void SetSceneAsActive(std::shared_ptr<Scene> scene);

		void Update(float deltaTime);
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;

		std::shared_ptr<Scene> m_ActiveScene;
	};
}
