#include "StateComponent.h"

#include "PlayerStateComponent.h"

StateComponent::StateComponent(unsigned displayWidth, unsigned displayHeight, unsigned playerDims, glm::vec2 playerSize)
	 : m_CurrentState(State::Idle)
	   , m_PreviousState(State::Nothing)
	   , m_PreviousStateBeforeThrowwing{State::Nothing}
	   , m_PreviousStateBeforeIdle{State::Nothing}
	   , m_SourcePath{ "BurgerTime/Mr.Pepper/" }
	   , m_PlayerSize{playerSize}
	   , m_PlayerDims{playerDims}
	   , m_WindowWidth(displayWidth)
	   , m_WindowHeight(displayHeight)
{
}

void StateComponent::Startup()
{
	Component::Startup();
}

void StateComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);
}
