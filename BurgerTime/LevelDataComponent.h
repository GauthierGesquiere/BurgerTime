#pragma once
#include <memory>

#include "DataComponent.h"

class LevelDataComponent : public DataComponent
{
public:
	LevelDataComponent(glm::vec2 sourceToDestRatio);
	~LevelDataComponent() override;
	LevelDataComponent(const LevelDataComponent& other) = delete;
	LevelDataComponent(LevelDataComponent&& other) = delete;
	LevelDataComponent& operator=(const LevelDataComponent& other) = delete;
	LevelDataComponent& operator=(LevelDataComponent&& other) = delete;

	void Startup() override;

	bool OnEvent(const dae::Event* event) override;

private:
	unsigned int m_Level{};

	std::shared_ptr<dae::GameObject> m_pLevelData{};

	unsigned int m_LevelWidth{ 7 };
	unsigned int m_LevelHeight{ 8 };

	glm::vec2 m_Position{};
};

