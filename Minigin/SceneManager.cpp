#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaSec)
{
	for (auto& scene : m_Scenes)
	{
		scene->Update(deltaSec);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	if (m_Scenes.size() == 0)
	{
		m_ActiveScene = scene;
	}

	m_Scenes.push_back(scene);
	return *scene;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetScene(const std::string& name) const
{
	for (auto& scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			return scene;
		}
	}
	return nullptr;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetActiveScene() const
{
	if (m_ActiveScene)
	{
		return m_ActiveScene;
	}

	std::cout << "No Active Scene" << std::endl;

	return nullptr;
}

void dae::SceneManager::SetSceneAsActive(const std::string& name)
{
	m_ActiveScene = GetScene(name);
}

void dae::SceneManager::SetSceneAsActive(std::shared_ptr<dae::Scene> scene)
{
	m_ActiveScene = scene;
}
