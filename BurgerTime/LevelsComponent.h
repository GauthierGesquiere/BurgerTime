#pragma once
#include <string>
#include <vector>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include <memory>

#include "Component.h"
#include "EventListener.h"
#include "Transform.h"

class LevelsComponent : public dae::Component, public dae::EventListener
{
public:
	LevelsComponent(unsigned int width, unsigned int height, unsigned int level = 1);
	~LevelsComponent() override;
	LevelsComponent(const LevelsComponent& other) = delete;
	LevelsComponent(LevelsComponent&& other) = delete;
	LevelsComponent& operator=(const LevelsComponent& other) = delete;
	LevelsComponent& operator=(LevelsComponent&& other) = delete;

	bool OnEvent(const dae::Event* event) override;

private:
	void Startup() override;
	void Update(float deltaSec) override;

	void LoadLevel(unsigned int levelIndex);
	void CreatePlayers(unsigned int amount);
	void CreateEnemy();
	void LoadData();
	void MakeBurger(int level);

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

	std::shared_ptr<dae::GameObject> m_pPlayer{};
	std::vector<std::shared_ptr<dae::GameObject>> m_pEnemies{};
	std::vector<std::shared_ptr<dae::GameObject>> m_pBurgers{};

	bool m_NeedsRestart{};
	bool m_GameOver{};

	float m_ElapsedSec{};
	float m_ElapsedSecEnemy{};
	bool m_LevelIsReset{};

	int m_AmountOfEnemies{};
	int m_MaxAmountOfEnemies{};

	int m_BurgersFinished{};

	int m_EnemiesInitialized{};
	bool m_BurgersInitialized{};
	std::vector <dae::Transform*> m_pEnemyTransforms{};
};

