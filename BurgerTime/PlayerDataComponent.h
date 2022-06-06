#pragma once
#include <memory>

#include "DataComponent.h"

class PlayerDataComponent : public DataComponent
{
public:
	PlayerDataComponent(glm::vec2 sourceToDestRatio);
	~PlayerDataComponent() override;
	PlayerDataComponent(const PlayerDataComponent& other) = delete;
	PlayerDataComponent(PlayerDataComponent&& other) = delete;
	PlayerDataComponent& operator=(const PlayerDataComponent& other) = delete;
	PlayerDataComponent& operator=(PlayerDataComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;
	bool OnEvent(const dae::Event* event) override;


private:
	unsigned int m_PlayerLives{};
	unsigned int m_playerScore{};

	bool m_GameIsReset{};

	std::shared_ptr<dae::GameObject> m_pPlayerData{};

	unsigned int m_LivesWidth{ 7 };
	unsigned int m_LivesHeight{ 8 };

	glm::vec2 m_Position{};
};

