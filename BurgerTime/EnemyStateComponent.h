#pragma once

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "Event.h"
#include "Observer.h"
#include "StateComponent.h"

enum class EnemyType
{
	Mr_HotDog,
	Mr_Pickle,
	Mr_Egg
};

class EnemyStateComponent : public StateComponent, public dae::Observer
{
public:
	EnemyStateComponent(unsigned int displayWidth, unsigned int displayHeight, unsigned int playerDims, glm::vec2 playerSize, EnemyType type);
	~EnemyStateComponent() override = default;
	EnemyStateComponent(const EnemyStateComponent& other) = delete;
	EnemyStateComponent(EnemyStateComponent&& other) = delete;
	EnemyStateComponent& operator=(const EnemyStateComponent& other) = delete;
	EnemyStateComponent& operator=(EnemyStateComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;

	void OnNotify(const dae::GameObject& actor, dae::Event* event) override;

private:
	void IsWalking(float deltaSec);
	void IsClimbing(float deltaSec);
	void IsDying(float deltaSec);

	EnemyType m_EnemyType{};
};

