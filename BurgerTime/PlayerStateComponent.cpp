#include "PlayerStateComponent.h"

#include <iostream>

#include "EventQueue.h"
#include "InputManager.h"

#include "GameObject.h"
#include "RenderSpriteComponent.h"

PlayerStateComponent::PlayerStateComponent(unsigned displayWidth, unsigned displayHeight, unsigned int playerDims, glm::vec2 playerSize)
	: StateComponent(displayWidth, displayHeight, playerDims, playerSize)
{
}

PlayerStateComponent::~PlayerStateComponent()
{
}

void PlayerStateComponent::Startup()
{
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		const std::string fullPath{ "Idle.png" };

		renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_Mirror);
		renderer->SetFreeze(2);
	}

	dae::EventQueue::GetInstance().Subscribe("KilledPlayer", this);
}

void PlayerStateComponent::Update(float deltaSec)
{
	switch (m_CurrentState)
	{
	case State::Walking:
		IsWalking(deltaSec);
		break;
	case State::Climbing:
		IsClimbing(deltaSec);
		break;
	case State::Throwing:
		IsThrowing(deltaSec);
		break;
	case State::Winning:
		IsWinning(deltaSec);
		break;
	case State::Idle:
		IsIdle(deltaSec);
		break;
	case State::Dying:
		IsDying(deltaSec);
		break;
	case State::Nothing: 
		break;
	default: 
		break;
	}
}

void PlayerStateComponent::OnNotify(const dae::GameObject& /*actor*/, dae::Event* event)
{
	if (m_IsDead)
	{
		delete event;
		return;
	}

	if (event->Message == "IsWalkingLeft")
	{
		m_CurrentState = State::Walking;

		//Was walking to the Right?
		if (m_Mirror == true)
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_Mirror = false;
	}
	else if (event->Message == "IsWalkingRight")
	{
		m_CurrentState = State::Walking;

		//Was walking to the Left?
		if ((m_Mirror == false))
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_Mirror = true;
	}
	else if (event->Message == "IsClimbingDown")
	{
		m_CurrentState = State::Climbing;

		//Was ClimbingUp?
		if (m_ClimbingUp == true)
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_ClimbingUp = false;
	}
	else if (event->Message == "IsClimbingUp")
	{
		m_CurrentState = State::Climbing;

		//Was ClimbingDown?
		if ((m_ClimbingUp == false))
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_ClimbingUp = true;
	}
	else if(event->Message == "IsIdle" && m_CurrentState != State::Throwing)
	{
		if (m_CurrentState != State::Idle)
		{
			//std::cout << "Idle" << std::endl;
			m_PreviousStateBeforeIdle = m_CurrentState;
			m_CurrentState = State::Idle;
		}
	}

	if (event->Message == "Throw")
	{
		m_PreviousStateBeforeThrowwing = m_CurrentState;
		m_CurrentState = State::Throwing;
	}

	//Important
	delete event;
}

bool PlayerStateComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "KilledPlayer")
	{
		ElapsedSec = 0.0f;
		std::cout << "test" << std::endl;
		m_CurrentState = State::Dying;
		m_IsDead = true;
		dae::EventQueue::GetInstance().Unsubscribe("KilledPlayer", this);
	}

	return false;
}

void PlayerStateComponent::IsWalking(float /*deltaSec*/)
{
	//if encountered ladder and pressed up/down then is climbing
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			const std::string fullPath{ "SideWalk.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_Mirror);
		}
	}
}

void PlayerStateComponent::IsClimbing(float /*deltaSec*/)
{
	//if entered ground and pressed left/right then is walking
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;

			std::string fullPath;
			if (m_ClimbingUp)
			{
				fullPath = "ClimbingUp.png";
			}
			else
			{
				fullPath = "ClimbingDown.png";
			}

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop);
		}
	}
}

void PlayerStateComponent::IsThrowing(float deltaSec)
{
	ElapsedSec += deltaSec;

	//wait 0.1f then back to previous state
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			ElapsedSec = 0.0f;

			const std::string fullPath{ "ThrowPepper.png" };

			if (m_PreviousState == State::Idle)
			{
				if (m_PreviousStateBeforeIdle == State::Walking)
				{
					m_FreezeOnFrame = 1;
				}
				else if (m_PreviousStateBeforeIdle == State::Climbing)
				{
					if (m_ClimbingUp)
					{
						m_FreezeOnFrame = 2;
					}
					else
					{
						m_FreezeOnFrame = 0;
					}
				}
			}

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.8f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop, m_FreezeOnFrame, m_Mirror);
			m_PreviousState = m_CurrentState;
		}

		if (ElapsedSec >= renderer->GetTotalAmountOfTime())
		{
			ElapsedSec = 0.0f;

			m_CurrentState = m_PreviousStateBeforeThrowwing;
		}
	}

}

void PlayerStateComponent::IsWinning(float /*deltaSec*/)
{
	//next level
}

void PlayerStateComponent::IsDying(float deltaSec)
{
	ElapsedSec += deltaSec;

	//restart level
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			const std::string fullPath{ "Death.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.3f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop);
		}

		if (ElapsedSec >= renderer->GetTotalAmountOfTime())
		{
			ElapsedSec = 0.0f;
			dae::EventQueue::GetInstance().Broadcast(new dae::Event("RestartLevel"));
		}
	}


}

void PlayerStateComponent::IsIdle(float /*deltaSec*/)
{
	//wait 0.1f then back to previous state
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			const std::string fullPath{ "Idle.png" };

			if (m_PreviousState == State::Climbing)
			{
				renderer->SetFreeze();
			}
			else
			{
				renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 1.0f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop);
			}
			m_PreviousState = m_CurrentState;
		}
	}
}
