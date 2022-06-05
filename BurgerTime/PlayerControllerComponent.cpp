#include "PlayerControllerComponent.h"

#include <iostream>

#include "GameObject.h"
#include "InputManager.h"
#include "PlayerStateComponent.h"
#include "Renderer.h"
#include "utils.h"
#include "MoveCommand.h"
#include "ThrowCommand.h"

PlayerControllerComponent::PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int playerDims, glm::vec2 playerSize)
	: m_pLevelIndices{ pLevelIndices }
	, m_Gravity{0, 9}
	, m_Velocity{0, 0}
	, m_PlayerSize{ playerSize }
	, m_PlayerDims{playerDims}
	, m_MovementSpeed{ 50 }
{
}

void PlayerControllerComponent::UpdateMovement(MoveDirections dir)
{
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

void PlayerControllerComponent::UpdateGravity(float deltaTime)
{
	m_Velocity += m_Gravity * deltaTime;
}

void PlayerControllerComponent::TranslateSprite(float deltaTime) const
{ 
	m_pOwner->GetTransform().MoveWith(glm::vec3{ m_Velocity.x * deltaTime, m_Velocity.y * deltaTime, 0 });
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

bool PlayerControllerComponent::HitFloor()
{
	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	const glm::vec2 startP{ box.left + box.width / 2, box.bottom - 2};
	const glm::vec2 endP{ box.left + box.width / 2, box.bottom + 2};

	/*
 *		+--------+
 *		|        |
 *		|        |
 *		|        |
 *		|    |   |
 *		|    |   |
 *		+----+---+
 *		     |
 */

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			return true;
		}
	}
	return false;
}

bool PlayerControllerComponent::HitEdge()
{
	if (m_Velocity.x == 0.0f )
	{
		return false;
	}

	bool hitEdge = false;

	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	glm::vec2 startP{ box.left + box.width / 2, box.bottom - 5 };
	glm::vec2 endP;

	if (m_Velocity.x > 0)
	{
	/*		Right
	 *		+--------+
	 *		|        |
	 *		|        |
	 *		|        |
	 *		|        |
	 *		|     ---+-
	 *		+--------+
	 */

		//IsWalkingRight
		endP = glm::vec2{ box.left + box.width + 1, box.bottom - 5 };
	}
	else if (m_Velocity.x < 0) 
	{
		/*		Left
		 *		+--------+
		 *		|        |
		 *		|        |
		 *		|        |
		 *		|        |
		 *	   -+---     |
		 *		+--------+
		 */

		//IsWalkingLeft
		endP = glm::vec2{ box.left - 1, box.bottom - 5 };
	}

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			hitEdge = true;
		}
	}

	//HitEdge but still need to do other check
	if (hitEdge)
	{
		hitEdge = false;
		startP = { box.left + box.width / 2, box.bottom - box.height + 10 };
		if (m_Velocity.x > 0)
		{
			/*		Right
			 *		+--------+
			 *		|     ---+-
			 *		|        |
			 *		|        |
			 *		|        |
			 *		|        |
			 *		+--------+
			 */

			 //IsWalkingRight
			endP = { box.left + box.width + 1, box.bottom - box.height + 10 };
		}
		else
		{
			/*		Left
			 *		+--------+
			 *	   -+---     |
			 *		|        |
			 *		|        |
			 *		|        |
			 *		|        |
			 *		+--------+
			 */

			//IsWalkingLeft
			endP = { box.left - 1, box.bottom - box.height + 10 };
		}

		for (const auto& levelIndices : *m_pLevelIndices)
		{
			if (Raycast(levelIndices, startP, endP, hitInfo))
			{
				hitEdge = true;
			}
		}

		if (hitEdge)
		{
			startP = { box.left + box.width / 2, box.bottom - box.height / 2 };
			if (m_Velocity.x > 0)
			{
				/*		Right
				 *		+--------+
				 *		|        |
				 *		|        |
				 *		|     ---+-
				 *		|        |
				 *		|        |
				 *		+--------+
				 */

				 //IsWalkingRight
				endP = { box.left + box.width + 1, box.bottom - box.height / 2 };
			}
			else
			{
				/*		Left
				 *		+--------+
				 *		|        |
				 *		|        |
				 *	   -+---     |
				 *		|        |
				 *		|        |
				 *		+--------+
				 */

				 //IsWalkingLeft
				endP = { box.left - 1, box.bottom - box.height / 2 };
			}

			for (const auto& levelIndices : *m_pLevelIndices)
			{
				if (Raycast(levelIndices, startP, endP, hitInfo))
				{
					hitEdge = false;
				}
			}

			if (hitEdge)
			{
				m_Velocity.x = 0.0f;
			}
		}
	}

	return hitEdge;
}

bool PlayerControllerComponent::HitLadder()
{
	bool hitLadder = false;
	m_HitLadder = false;

	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	glm::vec2 startP{ box.left + box.width / 2 - 10, box.bottom - 3 };
	glm::vec2 endP{ box.left + box.width / 2 + 10, box.bottom - 3 };

	/*
	 *		+--------+
	 *		|        |
	 *		|        |
	 *		|        |
	 *		|        |
	 *		| ------ |
	 *		+--------+
	 */


	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			hitLadder = true;
		}
	}

	//HitEdge but still need to do other check
	if (hitLadder)
	{
		startP = { box.left + box.width / 2 - 10, box.bottom - box.height +    5 };
		endP = { box.left + box.width / 2 + 10, box.bottom - box.height + 5 };

		/*
		 *		+--------+
		 *		| ------ |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		+--------+
		 */

		for (const auto& levelIndices : *m_pLevelIndices)
		{
			if (Raycast(levelIndices, startP, endP, hitInfo))
			{
				hitLadder = false;
			}
		}

		if (hitLadder)
		{
			//Can go only Down
			//std::cout << "Can Go Only Down" << std::endl;
			m_HitLadder = true;
			if (HitFloor())
			{
				m_LadderState = LadderState::Down;
			}
		}
		else
		{
			//Can go Up and Down
			//std::cout << "Can Go Up, Down" << std::endl;
			m_HitLadder = true;
			m_LadderState = LadderState::UpDown;
		}
	}
	else
	{
		startP = { box.left + box.width / 2 - 10, box.bottom - box.height + 5 };
		endP = { box.left + box.width / 2 + 10, box.bottom - box.height + 5 };

		/*
		 *		+--------+
		 *		| ------ |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		+--------+
		 */

		for (const auto& levelIndices : *m_pLevelIndices)
		{
			if (Raycast(levelIndices, startP, endP, hitInfo))
			{
				hitLadder = true;
			}
		}

		if (hitLadder)
		{
			//Can go only Up
			//std::cout << "Can Go Only Up" << std::endl;
			m_HitLadder = true;
			if (HitFloor())
			{
				m_LadderState = LadderState::Up;
			}
		}
		else
		{
			//m_LadderState = LadderState::NoLadder;
			//m_HitLadder = false;
		}
	}

	return hitLadder;
}

const utils::Rectf PlayerControllerComponent::CalculateBox() const
{
	utils::Rectf box;
	const glm::vec2 topLeft = m_pOwner->GetTransform().GetPosition();
	box.left = topLeft.x;
	box.bottom = topLeft.y + m_PlayerDims * m_PlayerSize.y;
	box.width = m_PlayerDims * m_PlayerSize.x;
	box.height = m_PlayerDims * m_PlayerSize.y;
	return box;
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
