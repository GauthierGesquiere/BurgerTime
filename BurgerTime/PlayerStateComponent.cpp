#include "PlayerStateComponent.h"

#include <iostream>

#include "InputManager.h"
#include "TestCommand.h"

#include <SDL_keycode.h>

#include "GameObject.h"
#include "RenderSpriteComponent.h"
#include "MoveCommand.h"

PlayerStateComponent::PlayerStateComponent(unsigned displayWidth, unsigned displayHeight, unsigned int playerDims, glm::vec2 playerSize)
	: m_CurrentPlayerState(PlayerState::Idle)
	  , m_PreviousPlayerState(PlayerState::Nothing)
	  , m_PreviousPlayerStateBeforeThrowwing{PlayerState::Nothing}
	  , m_PreviousPlayerStateBeforeIdle{PlayerState::Nothing}
	  , m_SourcePath{"BurgerTime/Mr.Pepper/"}
	  , m_PlayerSize{playerSize}
	  , m_PlayerDims{playerDims}
	  , m_WindowWidth(displayWidth)
	  , m_WindowHeight(displayHeight)
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
}

void PlayerStateComponent::Update(float deltaSec)
{
	switch (m_CurrentPlayerState)
	{
	case PlayerState::Walking:
		IsWalking(deltaSec);
		break;
	case PlayerState::Climbing:
		IsClimbing(deltaSec);
		break;
	case PlayerState::Throwing:
		IsThrowing(deltaSec);
		break;
	case PlayerState::Winning:
		IsWinning(deltaSec);
		break;
	case PlayerState::Idle:
		IsIdle(deltaSec);
		break;
	case PlayerState::Dying:
		IsDying(deltaSec);
		break;
	}
}

void PlayerStateComponent::OnNotify(const dae::GameObject& /*actor*/, dae::Event* event)
{
	//m_PreviousPlayerStateBeforeThrowwing = m_CurrentPlayerState;
	//m_CurrentPlayerState = PlayerState::Idle;

	if (event->Message == "IsWalkingLeft")
	{
		m_CurrentPlayerState = PlayerState::Walking;

		//Was walking to the Right?
		if (m_Mirror == true)
		{
			//ResetState
			m_PreviousPlayerState = PlayerState::Nothing;
		}
		m_Mirror = false;
	}
	else if (event->Message == "IsWalkingRight")
	{
		m_CurrentPlayerState = PlayerState::Walking;

		//Was walking to the Left?
		if ((m_Mirror == false))
		{
			//ResetState
			m_PreviousPlayerState = PlayerState::Nothing;
		}
		m_Mirror = true;
	}
	else if (event->Message == "IsClimbingDown")
	{
		m_CurrentPlayerState = PlayerState::Climbing;

		//Was ClimbingUp?
		if (m_ClimbingUp == true)
		{
			//ResetState
			m_PreviousPlayerState = PlayerState::Nothing;
		}
		m_ClimbingUp = false;
	}
	else if (event->Message == "IsClimbingUp")
	{
		m_CurrentPlayerState = PlayerState::Climbing;

		//Was ClimbingDown?
		if ((m_ClimbingUp == false))
		{
			//ResetState
			m_PreviousPlayerState = PlayerState::Nothing;
		}
		m_ClimbingUp = true;
	}
	else if(event->Message == "IsIdle" && m_CurrentPlayerState != PlayerState::Throwing)
	{
		if (m_CurrentPlayerState != PlayerState::Idle)
		{
			//std::cout << "Idle" << std::endl;
			m_PreviousPlayerStateBeforeIdle = m_CurrentPlayerState;
			m_CurrentPlayerState = PlayerState::Idle;
		}
	}

	if (event->Message == "Throw")
	{
		m_PreviousPlayerStateBeforeThrowwing = m_CurrentPlayerState;
		m_CurrentPlayerState = PlayerState::Throwing;
	}

	//Important
	delete event;
}

void PlayerStateComponent::IsWalking(float /*deltaSec*/)
{
	//if encountered ladder and pressed up/down then is climbing
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousPlayerState != m_CurrentPlayerState)
		{
			m_PreviousPlayerState = m_CurrentPlayerState;
			const std::string fullPath{ "SideWalk.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_Mirror);
		}
	}

	/*if (ElapsedSec >= 10)
	{
		ElapsedSec = 0.0f;

		m_PreviousPlayerStateBeforeThrowwing = m_CurrentPlayerState;
		m_CurrentPlayerState = PlayerState::Throwing;
	}*/
}

void PlayerStateComponent::IsClimbing(float /*deltaSec*/)
{
	//if entered ground and pressed left/right then is walking
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousPlayerState != m_CurrentPlayerState)
		{
			m_PreviousPlayerState = m_CurrentPlayerState;

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
		if (m_PreviousPlayerState != m_CurrentPlayerState)
		{
			const std::string fullPath{ "ThrowPepper.png" };

			if (m_PreviousPlayerState == PlayerState::Idle)
			{
				if (m_PreviousPlayerStateBeforeIdle == PlayerState::Walking)
				{
					m_FreezeOnFrame = 1;
				}
				else if (m_PreviousPlayerStateBeforeIdle == PlayerState::Climbing)
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
			m_PreviousPlayerState = m_CurrentPlayerState;
		}

		if (ElapsedSec >= renderer->GetTotalAmountOfTime())
		{
			ElapsedSec = 0.0f;

			m_CurrentPlayerState = m_PreviousPlayerStateBeforeThrowwing;
		}
	}

}

void PlayerStateComponent::IsWinning(float /*deltaSec*/)
{
	//next level
}

void PlayerStateComponent::IsDying(float /*deltaSec*/)
{
	//restart level
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousPlayerState != m_CurrentPlayerState)
		{
			m_PreviousPlayerState = m_CurrentPlayerState;
			const std::string fullPath{ "Death.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 1.f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop);
		}

		if (ElapsedSec >= renderer->GetTotalAmountOfTime())
		{
			ElapsedSec = 0.0f;

			m_CurrentPlayerState = m_PreviousPlayerState;
		}
	}


}

void PlayerStateComponent::IsIdle(float /*deltaSec*/)
{
	//wait 0.1f then back to previous state
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousPlayerState != m_CurrentPlayerState)
		{
			const std::string fullPath{ "Idle.png" };

			if (m_PreviousPlayerState == PlayerState::Climbing)
			{
				renderer->SetFreeze();
			}
			else
			{
				renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 1.0f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop);
			}
			m_PreviousPlayerState = m_CurrentPlayerState;

		}
	}
}
void PlayerStateComponent::InitInput()
{
	//auto& input = dae::InputManager::GetInstance();
	//input.ssss(0, dae::ControllerButton::GAMEPAD_A, new dae::TestCommand(), dae::InputManager::InputState::Pressed);
	//input.SetCommandToKey(0, SDLK_q, new dae::TestCommand(), dae::InputManager::InputState::Pressed);

}