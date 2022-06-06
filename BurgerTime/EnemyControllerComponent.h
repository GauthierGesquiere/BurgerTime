#pragma once
#include "Component.h"

#include <vector>
#include "Subject.h"
#include "ControllerComponent.h"
#include "Transform.h"


#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>

#include "EventListener.h"


#pragma warning(pop)


class EnemyControllerComponent : public ControllerComponent, public dae::Subject
{
public:
	EnemyControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int enemyDims, glm::vec2 enemySize, glm::vec2 spawnPoint);
	~EnemyControllerComponent() override = default;
	EnemyControllerComponent(const EnemyControllerComponent& other) = delete;
	EnemyControllerComponent(EnemyControllerComponent&& other) = delete;
	EnemyControllerComponent& operator=(const EnemyControllerComponent& other) = delete;
	EnemyControllerComponent& operator=(EnemyControllerComponent&& other) = delete;

	void SetPlayerTransform(dae::Transform* playerTransform);

	bool m_IsInitialized{};

private:
	void Startup() override;
	void Update(float deltaSec) override;

	void UpdateLeft();
	void UpdateRight();
	void UpdateDown();
	void UpdateUp();

	void UpdateAILogic(float deltaSec);
	void UpdateIfKilledMrPepper();
	void GetUpdateMethod();

	dae::Transform* m_pPlayerTransform{};

	bool m_IsEnemyGoingUp{};
	bool m_IsEnemyGoingRight{};
	bool m_FirstUpdate{};

	bool m_HitJunction{};
	bool m_IsStunned{};
	bool m_IsPlayerKilled{};
	
	enum class NeedUpdate
	{
		Up,
		Down,
		Left,
		Right,
		None
	};

	NeedUpdate m_NeedsUpdateHorizontal{};
	NeedUpdate m_NeedsUpdateVertical{};
	NeedUpdate m_PreviousUpdateVertical{};

	glm::vec2 m_SpawnPoint{};
};


