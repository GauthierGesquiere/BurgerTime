#include "LevelsComponent.h"
#include <memory>

#include "GameObject.h"
#include "PlayerStateComponent.h"
#include "RenderSpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"

LevelsComponent::LevelsComponent(unsigned int width, unsigned int height, unsigned int level)
	: m_level{level}
	, m_SourcePath{"BurgerTime/"}
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	//LoadLevel(m_level);
	CreatePlayers(1);
}

void LevelsComponent::Update(float /*deltaSec*/)
{

}

void LevelsComponent::LoadLevel(unsigned levelIndex)
{
	std::string fullPath{ "Levels/Level" };
	fullPath += std::to_string(levelIndex) + ".png";

	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent(m_SourcePath + fullPath, m_LevelWidth, m_LevelHeight, m_WindowWidth, m_WindowHeight));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
}

void LevelsComponent::CreatePlayers(unsigned amount)
{
	for (unsigned int i = 0; i < amount; ++i)
	{
		const auto gObject = std::make_shared<dae::GameObject>();
		gObject->AddComponent(new RenderSpriteComponent());
		gObject->AddComponent(new PlayerStateComponent(m_WindowWidth, m_WindowHeight));
		dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	}
}
