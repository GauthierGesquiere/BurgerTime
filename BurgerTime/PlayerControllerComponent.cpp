#include "PlayerControllerComponent.h"

#include <iostream>

#include "EventQueue.h"
#include "GameObject.h"
#include "InputManager.h"
#include "PlayerStateComponent.h"
#include "Renderer.h"
#include "utils.h"
#include "MoveCommand.h"
#include "ThrowCommand.h"

PlayerControllerComponent::PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int playerDims, glm::vec2 playerSize)
	: ControllerComponent(pLevelIndices, playerDims, playerSize)
{
	dae::EventQueue::GetInstance().Subscribe("KilledPlayer", this);

}

PlayerControllerComponent::~PlayerControllerComponent()
{
	 dae::InputManager::GetInstance().RemoveCommands();
}

void PlayerControllerComponent::UpdateMovement(MoveDirections dir)
{
	if (m_IsDead)
	{
		return;
	}

	switch (dir)
	{
	case MoveDirections::Left:
		UpdateLeft();
		break;
	case MoveDirections::Right:
		UpdateRight();
		break;
	case MoveDirections::Down:
		UpdateDown();
		break;
	case MoveDirections::Up:
		UpdateUp();
		break;
	}
}

void PlayerControllerComponent::ThrowSalt() const
{
	Notify(*m_pOwner, new dae::Event("Throw"));
}

bool PlayerControllerComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "KilledPlayer")
	{
		m_IsDead = true;
		dae::EventQueue::GetInstance().Unsubscribe("KilledPlayer", this);
	}
	return false;
}

void PlayerControllerComponent::Startup()
{
	AddInput();
	m_pOwner->GetTransform().SetPosition( 297,  423, 0 );
	AddObserver(m_pOwner->GetComponentOfType<PlayerStateComponent>());
}

void PlayerControllerComponent::Update(float deltaSec)
{
	//UpdateGravity(deltaSec);
	if (HitFloor() && !m_HitLadder) 
	{
		m_Velocity.y = 0.0f;
	}



	//std::cout << m_HitLadder << std::endl;

	HitLadder();
	HitEdge();
	TranslateSprite(deltaSec);
	UpdateReset();
}

void PlayerControllerComponent::UpdateLeft()
{
	if (HitFloor())
	{
		Notify(*m_pOwner, new dae::Event("IsWalkingLeft"));
		m_Velocity.x = -m_MovementSpeed;
	}
}

void PlayerControllerComponent::UpdateRight()
{
	if (HitFloor())
	{
		Notify(*m_pOwner, new dae::Event("IsWalkingRight"));
		m_Velocity.x = m_MovementSpeed;
	}
}

void PlayerControllerComponent::UpdateDown()
{
	if (m_HitLadder && (m_LadderState == LadderState::Down || m_LadderState == LadderState::UpDown))
	{
		Notify(*m_pOwner, new dae::Event("IsClimbingDown"));
		m_Velocity.y = m_MovementSpeed;
	}
}

void PlayerControllerComponent::UpdateUp()
{
	if (m_HitLadder && (m_LadderState == LadderState::Up || m_LadderState == LadderState::UpDown))
	{
		Notify(*m_pOwner, new dae::Event("IsClimbingUp"));
		m_Velocity.y = -m_MovementSpeed;
	}
}

void PlayerControllerComponent::UpdateReset()
{
	if (m_Velocity.x == 0.0f && m_Velocity.y == 0.0f)
	{
		Notify(*m_pOwner, new dae::Event("IsIdle"));
	}

	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
}

void PlayerControllerComponent::AddInput()
{
	auto& input = dae::InputManager::GetInstance();
	input.SetCommandToKey(0, SDLK_a, new MoveCommand(this, MoveDirections::Left), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_d, new MoveCommand(this, MoveDirections::Right), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_s, new MoveCommand(this, MoveDirections::Down), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_w, new MoveCommand(this, MoveDirections::Up), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_l, new ThrowCommand(this), dae::InputManager::InputState::Pressed);


	//input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_UP, new MoveCommand(this, MoveDirections::Left), dae::InputManager::InputState::Hold);
	//input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_LEFT, new MoveCommand(this, MoveDirections::Right), dae::InputManager::InputState::Hold);
	//input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_RIGHT, new MoveCommand(this, MoveDirections::Down), dae::InputManager::InputState::Hold);
	//input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_DOWN, new MoveCommand(this, MoveDirections::Up), dae::InputManager::InputState::Hold);
	//input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_A, new ThrowCommand(this), dae::InputManager::InputState::Pressed);
}
