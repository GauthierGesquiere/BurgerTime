#pragma once
#include <string>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "Component.h"

enum class PlayerState
{
	Walking,
	Climbing,
	Throwing,
	Winning,
	Dying,
	Nothing
};

class PlayerStateComponent : public dae::Component
{
public:
	PlayerStateComponent(unsigned int displayWidth, unsigned int displayHeight, unsigned int playerDims, glm::vec2 playerSize);
	~PlayerStateComponent() override = default;
	PlayerStateComponent(const PlayerStateComponent& other) = delete;
	PlayerStateComponent(PlayerStateComponent&& other) = delete;
	PlayerStateComponent& operator=(const PlayerStateComponent& other) = delete;
	PlayerStateComponent& operator=(PlayerStateComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;

private:
	PlayerState m_CurrentPlayerState{};
	PlayerState m_PreviousPlayerState{};
	PlayerState m_PreviousPlayerStateBeforeThrowwing{};

	std::string m_SourcePath{};

	glm::vec2 m_PlayerSize{};
	unsigned int m_PlayerDims{};

	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight{};

	bool m_Mirror{ false };
	float ElapsedSec{};

	int m_FreezeOnFrame{};

	void IsWalking(float deltaSec);
	void IsClimbing(float deltaSec);
	void IsThrowing(float deltaSec);
	void IsWinning(float deltaSec);
	void IsDying(float deltaSec);

	void InitInput();

};



