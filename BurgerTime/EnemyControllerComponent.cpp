#include "EnemyControllerComponent.h"

#include <iostream>

#include "EnemyStateComponent.h"
#include "EventQueue.h"
#include "GameObject.h"

EnemyControllerComponent::EnemyControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned enemyDims, glm::vec2 enemySize)
	: ControllerComponent(pLevelIndices, enemyDims, enemySize)
	, m_NeedsUpdateHorizontal{ NeedUpdate::None }
	, m_NeedsUpdateVertical{ NeedUpdate::None }
	, m_FirstUpdate{ true }
{
	m_MovementSpeed = 25.0f;
}

void EnemyControllerComponent::SetPlayerTransform(dae::Transform* playerTransform)
{
	m_pPlayerTransform = playerTransform;
}

void EnemyControllerComponent::Startup()
{
	m_pOwner->GetTransform().SetPosition(20, 423, 0);
	AddObserver(m_pOwner->GetComponentOfType<EnemyStateComponent>());
}

void EnemyControllerComponent::Update(float deltaSec)
{
	UpdateGravity(deltaSec);

	m_HitJunction = false;
	if (HitFloor() && m_HitLadder)
	{
		m_HitJunction = true;
	}

	if (m_FirstUpdate)
	{
		GetUpdateMethod();
	}
	else
	{
		UpdateIfKilledMrPepper();
	}

	if (HitFloor() && !m_HitLadder)
	{
		m_Velocity.y = 0.0f;
	}

	HitLadder();
	if (HitEdge())
	{
		if (m_NeedsUpdateHorizontal == NeedUpdate::Left)
		{
			m_NeedsUpdateHorizontal = NeedUpdate::Right;
		}
		if (m_NeedsUpdateHorizontal == NeedUpdate::Right)
		{
			m_NeedsUpdateHorizontal = NeedUpdate::Left;
		}
	}
	UpdateAILogic(deltaSec);
	TranslateSprite(deltaSec);

	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
}

void EnemyControllerComponent::UpdateLeft()
{
	if (HitFloor())
	{
		Notify(*m_pOwner, new dae::Event("IsWalkingLeft"));
		m_Velocity.x = -m_MovementSpeed;
		m_IsEnemyGoingRight = false;
		m_PreviousUpdateVertical = NeedUpdate::None;
	}
}

void EnemyControllerComponent::UpdateRight()
{
	if (HitFloor())
	{
		Notify(*m_pOwner, new dae::Event("IsWalkingRight"));
		m_Velocity.x = m_MovementSpeed;
		m_IsEnemyGoingRight = true;
		m_PreviousUpdateVertical = NeedUpdate::None;
	}
}

void EnemyControllerComponent::UpdateDown()
{
	if (m_HitLadder && (m_LadderState == LadderState::Down || m_LadderState == LadderState::UpDown))
	{
		Notify(*m_pOwner, new dae::Event("IsClimbingDown"));
		m_Velocity.y = m_MovementSpeed;
		m_IsEnemyGoingUp = false;
		m_PreviousUpdateVertical = NeedUpdate::Down;
	}
}

void EnemyControllerComponent::UpdateUp()
{
	if (m_HitLadder && (m_LadderState == LadderState::Up || m_LadderState == LadderState::UpDown))
	{
		Notify(*m_pOwner, new dae::Event("IsClimbingUp"));
		m_Velocity.y = -m_MovementSpeed;
		m_IsEnemyGoingUp = true;
		m_PreviousUpdateVertical = NeedUpdate::Up;
	}
}

void EnemyControllerComponent::UpdateAILogic(float /*deltaSec*/)
{
	if (m_HitJunction)
	{
		GetUpdateMethod();
	}

	switch (m_NeedsUpdateHorizontal)
	{
	case NeedUpdate::Left:
		UpdateLeft();
		break;
	case NeedUpdate::Right:
		UpdateRight();
		break;
	case NeedUpdate::None: 
		break;
	}


	switch (m_NeedsUpdateVertical)
	{
	case NeedUpdate::Up:
		UpdateUp();
		break;
	case NeedUpdate::Down:
		UpdateDown();
		break;
	case NeedUpdate::None: 
		break;
	}
	
}

void EnemyControllerComponent::UpdateIfKilledMrPepper()
{
	if (m_IsStunned || m_IsPlayerKilled)
	{
		return;
	}

	const auto boxPlayer = CalculateBox(m_pPlayerTransform->GetPosition());
	const auto boxEnemy = CalculateBox();

	if (IsOverlapping(boxPlayer, boxEnemy))
	{
		dae::EventQueue::GetInstance().Broadcast(new dae::Event("KilledPlayer"));
		m_IsPlayerKilled = true;
	}
}

void EnemyControllerComponent::GetUpdateMethod()
{
	if (!m_pPlayerTransform)
	{
		std::cout << "did not find players transform" << std::endl;
		return;
	}

	m_FirstUpdate = false;

	const auto& enemyPos = m_pOwner->GetTransform().GetPosition();
	const auto& playerPos = m_pPlayerTransform->GetPosition();

	const auto& length = playerPos - enemyPos;

	if (abs(length.x) < abs(length.y))
	{
		if (enemyPos.y >= playerPos.y && (m_LadderState == LadderState::Up || m_LadderState == LadderState::UpDown) && m_PreviousUpdateVertical == NeedUpdate::None)
		{
			//Then the enemy needs to go up
			m_NeedsUpdateVertical = NeedUpdate::Up;
		}
		else if (enemyPos.y < playerPos.y && (m_LadderState == LadderState::Down || m_LadderState == LadderState::UpDown) && m_PreviousUpdateVertical == NeedUpdate::None)
		{
			//Then the enemy needs to go down
			m_NeedsUpdateVertical = NeedUpdate::Down;
			//m_PreviousUpdateVertical = NeedUpdate::Down;
		}
	}

	if (enemyPos.x >= playerPos.x)
	{
		//Then the enemy needs to go left
		m_NeedsUpdateHorizontal = NeedUpdate::Left;
	}
	else if (enemyPos.x < playerPos.x)
	{
		//Then the enemy needs to go right
		m_NeedsUpdateHorizontal = NeedUpdate::Right;
	}
	
}
