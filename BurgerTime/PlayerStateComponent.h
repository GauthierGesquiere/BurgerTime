#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "EventListener.h"
#include "Observer.h"
#include "StateComponent.h"

class PlayerStateComponent : public StateComponent, public dae::Observer, public dae::EventListener
{
public:
	PlayerStateComponent(unsigned int displayWidth, unsigned int displayHeight, unsigned int playerDims, glm::vec2 playerSize);
	~PlayerStateComponent() override;
	PlayerStateComponent(const PlayerStateComponent& other) = delete;
	PlayerStateComponent(PlayerStateComponent&& other) = delete;
	PlayerStateComponent& operator=(const PlayerStateComponent& other) = delete;
	PlayerStateComponent& operator=(PlayerStateComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;

	void OnNotify(const dae::GameObject& actor, dae::Event* event) override;
	bool OnEvent(const dae::Event* event) override; 

private:
	void IsWalking(float deltaSec);
	void IsClimbing(float deltaSec);
	void IsThrowing(float deltaSec);
	void IsWinning(float deltaSec);
	void IsDying(float deltaSec);
	void IsIdle(float deltaSec);

	bool m_IsDead{};
};





