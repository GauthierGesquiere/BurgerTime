#pragma once
#include "Component.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <memory>
#include <glm/glm.hpp>

#include "Transform.h"
#pragma warning(pop)

class BurgerComponent : public dae::Component
{
public:
	BurgerComponent(int level, int column, glm::vec2 sourceToDestRatio, std::vector<std::vector<glm::vec2>>* pLevelIndices);
	~BurgerComponent() override = default;
	BurgerComponent(const BurgerComponent& other) = delete;
	BurgerComponent(BurgerComponent&& other) = delete;
	BurgerComponent& operator=(const BurgerComponent& other) = delete;
	BurgerComponent& operator=(BurgerComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;

	void SetPlayerTransform(dae::Transform* pPlayerTransform);
	void SetAllEnemies(std::vector<std::shared_ptr<dae::GameObject>>& pEnemies);

private:
	void MakeBurger();
	void CheckIfHitEnemy();

	int m_Level{};
	int m_Column{};

	glm::vec2 m_SourceToDestRatio{};

	std::vector<std::shared_ptr<dae::GameObject>> m_BurgerPieces{};
	std::vector<std::vector<glm::vec2>>* m_pLevelIndices{};

	float m_PosX{};

	std::vector<std::shared_ptr<dae::GameObject>> m_pEnemies{};
};

