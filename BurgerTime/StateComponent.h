#pragma once
#include "Component.h"

#include <string>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

enum class State
{
	Walking,
	Climbing,
	Throwing,
	Winning,
	Dying,
	Idle,
	Nothing
};

class StateComponent : public dae::Component
{
public:
	StateComponent(unsigned int displayWidth, unsigned int displayHeight, unsigned int playerDims, glm::vec2 playerSize);
	~StateComponent() override = default;
	StateComponent(const StateComponent& other) = delete;
	StateComponent(StateComponent&& other) = delete;
	StateComponent& operator=(const StateComponent& other) = delete;
	StateComponent& operator=(StateComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;


protected:
	State m_CurrentState{};
	State m_PreviousState{};
	State m_PreviousStateBeforeThrowwing{};
	State m_PreviousStateBeforeIdle{};

	std::string m_SourcePath{};

	glm::vec2 m_PlayerSize{};
	unsigned int m_PlayerDims{};

	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight{};

	bool m_ClimbingUp{};
	bool m_Mirror{ false };
	float ElapsedSec{};

	int m_FreezeOnFrame{};
};

