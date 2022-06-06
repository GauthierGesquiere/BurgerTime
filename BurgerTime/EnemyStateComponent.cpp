#include "EnemyStateComponent.h"

#include "GameObject.h"
#include "RenderSpriteComponent.h"

EnemyStateComponent::EnemyStateComponent(unsigned displayWidth, unsigned displayHeight, unsigned playerDims, glm::vec2 playerSize, EnemyType type)
	: StateComponent(displayWidth, displayHeight, playerDims, playerSize)
	, m_EnemyType{ type }
{
}

void EnemyStateComponent::Startup()
{
	StateComponent::Startup();

	if (m_EnemyType == EnemyType::Mr_HotDog)
	{
		m_SourcePath = "BurgerTime/Enemies/Hotdog_";
	}
	else if (m_EnemyType == EnemyType::Mr_Egg)
	{
		m_SourcePath = "BurgerTime/Enemies/Egg_";
	}
	else if (m_EnemyType == EnemyType::Mr_Pickle)
	{
		m_SourcePath = "BurgerTime/Enemies/Pickle_";
	}
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		renderer->SetTextureToDraw(m_SourcePath + "Walk.png", m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_Mirror);
		renderer->SetFreeze(2);
	}
}

void EnemyStateComponent::Update(float deltaSec)
{
	switch (m_CurrentState)
	{
	case State::Walking:
		IsWalking(deltaSec);
		break;
	case State::Climbing:
		IsClimbing(deltaSec);
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

void EnemyStateComponent::OnNotify(const dae::GameObject& /*actor*/, dae::Event* event)
{
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
	else if (event->Message == "IsIdle" && m_CurrentState != State::Throwing)
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

void EnemyStateComponent::IsWalking(float /*deltaSec*/)
{
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			const std::string fullPath{ "Walk.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_Mirror);
		}
	}
}

void EnemyStateComponent::IsClimbing(float /*deltaSec*/)
{
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

void EnemyStateComponent::IsDying(float /*deltaSec*/)
{
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			const std::string fullPath{ "Death.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop);
		}
	}
}

