#include "PlayerStateComponent.h"

#include "InputManager.h"
#include "TestCommand.h"

#include <SDL_keycode.h>

#include "GameObject.h"
#include "RenderSpriteComponent.h"

PlayerStateComponent::PlayerStateComponent(unsigned displayWidth, unsigned displayHeight)
	: m_SourcePath{"BurgerTime/Mr.Pepper/"}
	  , m_PlayerDims(16)
	  , m_WindowWidth(displayWidth)
	  , m_WindowHeight(displayHeight)
	  , m_PreviousPlayerState(PlayerState::Nothing)
{
}

void PlayerStateComponent::Startup()
{
	Component::Startup();
}

void PlayerStateComponent::Update(float deltaSec)
{
	ElapsedSec += deltaSec;

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
	case PlayerState::Dying:
		IsDying(deltaSec);
		break;
	}

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

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_WindowWidth, m_WindowHeight, LoopType::ForwardReverseLoop, -1, m_Mirror);
		}
	}

	if (ElapsedSec >= 10)
	{
		ElapsedSec = 0.0f;

		m_PreviousPlayerStateBeforeThrowwing = m_CurrentPlayerState;
		m_CurrentPlayerState = PlayerState::Throwing;
	}
}

void PlayerStateComponent::IsClimbing(float /*deltaSec*/)
{
	//if entered ground and pressed left/right then is walking
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousPlayerState != m_CurrentPlayerState)
		{
			m_PreviousPlayerState = m_CurrentPlayerState;
			const std::string fullPath{ "ClimbingDown.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_WindowWidth, m_WindowHeight, LoopType::ForwardReverseLoop);
		}
	}

	if (ElapsedSec >= 10)
	{
		ElapsedSec = 0.0f;

		m_PreviousPlayerStateBeforeThrowwing = m_CurrentPlayerState;
		m_CurrentPlayerState = PlayerState::Throwing;
	}
}

void PlayerStateComponent::IsThrowing(float /*deltaSec*/)
{
	//wait 0.1f then back to previous state
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousPlayerState != m_CurrentPlayerState)
		{
			const std::string fullPath{ "ThrowPepper.png" };

			if (m_PreviousPlayerState == PlayerState::Walking)
			{
				m_FreezeOnFrame = 1;
			}
			else if (m_PreviousPlayerState == PlayerState::Climbing)
			{
				m_FreezeOnFrame = 0;
			}

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 1.0f, m_WindowWidth, m_WindowHeight, LoopType::NoLoop, m_FreezeOnFrame, m_Mirror);
			m_PreviousPlayerState = m_CurrentPlayerState;
		}

		if (ElapsedSec >= renderer->GetTotalAmountOfTime())
		{
			ElapsedSec = 0.0f;

			m_CurrentPlayerState = m_PreviousPlayerStateBeforeThrowwing;
			m_Mirror = !m_Mirror;
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

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 1.f, m_WindowWidth, m_WindowHeight, LoopType::NoLoop);
		}

		if (ElapsedSec >= renderer->GetTotalAmountOfTime())
		{
			ElapsedSec = 0.0f;

			m_CurrentPlayerState = m_PreviousPlayerState;
		}
	}


}

void PlayerStateComponent::InitInput()
{
	auto& input = dae::InputManager::GetInstance();
	input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_A, new dae::TestCommand(), dae::InputManager::InputState::KeyDown);
	input.SetCommandToKey(0, SDLK_q, new dae::TestCommand(), dae::InputManager::InputState::KeyDown);
}