#include "PlayerDataComponent.h"

#include <iostream>

#include "EventQueue.h"
#include "GameObject.h"
#include "RenderSpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"

PlayerDataComponent::PlayerDataComponent(glm::vec2 sourceToDestRatio)
	: DataComponent(sourceToDestRatio)
	, m_PlayerLives{ 3 }
	, m_GameIsReset{ true }
	, m_Position{10, 550}
{
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent(glm::vec2{ m_Position.x, m_Position.y }));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_pPlayerData = gObject;
}

PlayerDataComponent::~PlayerDataComponent()
{
}

void PlayerDataComponent::Startup()
{
	dae::EventQueue::GetInstance().Subscribe("KilledPlayer", this);
	dae::EventQueue::GetInstance().Subscribe("LevelIsReset", this);

	const std::string fullPath = "Lives.png";

	m_pPlayerData->GetComponentOfType<RenderSpriteComponent>()->SetTextureToDraw(
		m_SourcePath + fullPath, m_LivesWidth, m_LivesHeight * (m_PlayerLives - 1), 0,
		m_LivesWidth * static_cast<unsigned int>(m_SourceToDestRatio.x),
		m_LivesHeight * (m_PlayerLives - 1) * static_cast<unsigned int>(m_SourceToDestRatio.y), LoopType::Loop, 1);
}

void PlayerDataComponent::Update(float /*deltaSec*/)
{
	if (m_PlayerLives == 0)
	{
		dae::EventQueue::GetInstance().Broadcast(new dae::Event("GameOver"));
	}
}

bool PlayerDataComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "KilledPlayer" && m_GameIsReset)
	{
		//std::cout << m_PlayerLives << std::endl;
		m_GameIsReset = false;
		m_PlayerLives--;

		if (m_PlayerLives == 2)
		{
			m_pPlayerData->GetComponentOfType<RenderSpriteComponent>()->SetPosition(glm::vec2(m_Position.x, m_Position.y + m_PlayerLives + 3 * m_LivesHeight));
		}
		if (m_PlayerLives == 1)
		{
			m_pPlayerData->GetComponentOfType<RenderSpriteComponent>()->SetPosition(glm::vec2(m_Position.x, m_Position.y + (m_PlayerLives - 4) * m_LivesHeight));
		}

		const std::string fullPath = "Lives.png";

		if (m_PlayerLives > 2)
		{
			m_pPlayerData->GetComponentOfType<RenderSpriteComponent>()->SetTextureToDraw(
				m_SourcePath + fullPath, m_LivesWidth, m_LivesHeight * (m_PlayerLives - 1), 0,
				m_LivesWidth * static_cast<unsigned int>(m_SourceToDestRatio.x),
				m_LivesHeight * (m_PlayerLives) * static_cast<unsigned int>(m_SourceToDestRatio.y), LoopType::Loop, 1);
		}
	}
	if (event->Message == "LevelIsReset")
	{
		m_GameIsReset = true;
	}

	return false;
}
