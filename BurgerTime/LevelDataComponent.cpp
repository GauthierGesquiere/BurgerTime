#include "LevelDataComponent.h"

#include "EventQueue.h"
#include "GameObject.h"
#include "RenderSpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"

LevelDataComponent::LevelDataComponent(glm::vec2 sourceToDestRatio)
	: DataComponent(sourceToDestRatio)
	, m_Level{ 1 }
	, m_Position{ 605, 570 }

{
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent(glm::vec2{ m_Position.x, m_Position.y }));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_pLevelData = gObject;
}

LevelDataComponent::~LevelDataComponent()
{
}

void LevelDataComponent::Startup()
{
	dae::EventQueue::GetInstance().Subscribe("AddScore", this);

	const std::string fullPath = "Levels.png";

	m_pLevelData->GetComponentOfType<RenderSpriteComponent>()->SetTextureToDraw(
		m_SourcePath + fullPath, m_LevelWidth, m_LevelHeight, 0,
		m_LevelWidth * static_cast<unsigned int>(m_SourceToDestRatio.x),
		m_LevelHeight * static_cast<unsigned int>(m_SourceToDestRatio.y), LoopType::NoLoop, 1);

}

bool LevelDataComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "NextLevel")
	{
		m_Level++;

		if (m_Level == 2)
		{
			m_pLevelData->GetComponentOfType<RenderSpriteComponent>()->SetPosition(glm::vec2(m_Position.x, m_Position.y + m_Level + 3 * m_LevelHeight));
		}
		if (m_Level == 1)
		{
			m_pLevelData->GetComponentOfType<RenderSpriteComponent>()->SetPosition(glm::vec2(m_Position.x, m_Position.y + (m_Level - 4) * m_LevelHeight));
		}

		const std::string fullPath = "Lives.png";

		m_pLevelData->GetComponentOfType<RenderSpriteComponent>()->SetTextureToDraw(
			m_SourcePath + fullPath, m_LevelWidth, m_LevelHeight, 0,
			m_LevelWidth * static_cast<unsigned int>(m_SourceToDestRatio.x),
			m_LevelHeight * static_cast<unsigned int>(m_SourceToDestRatio.y), LoopType::NoLoop, 1);
	}

	return false;
}
