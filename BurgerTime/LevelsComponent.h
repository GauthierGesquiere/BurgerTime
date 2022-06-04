#pragma once
#include <string>
#include <vector>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "Component.h"

class LevelsComponent : public dae::Component
{
public:
	LevelsComponent(unsigned int width, unsigned int height, unsigned int level = 1);
	~LevelsComponent() override = default;
	LevelsComponent(const LevelsComponent& other) = delete;
	LevelsComponent(LevelsComponent&& other) = delete;
	LevelsComponent& operator=(const LevelsComponent& other) = delete;
	LevelsComponent& operator=(LevelsComponent&& other) = delete;

private:
	void Startup() override;
	void Update(float deltaSec) override;

	void LoadLevel(unsigned int levelIndex);
	void CreatePlayers(unsigned int amount);

	//Dimensions
	unsigned int m_LevelWidth{ 208 };
	unsigned int m_LevelHeight{ 200 };

	unsigned int m_PlayerDims{ 16 };

	unsigned int m_BurgerWidth{ 31 };
	unsigned int m_BurgerHeight{ 8 };

	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight{};

	unsigned int m_level{};

	std::string m_SourcePath{};

	std::vector<std::vector<glm::vec2>> m_LevelVertices{};

	glm::vec2 m_SourceToDestRatio{};
};
