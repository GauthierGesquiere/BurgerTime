#include "PlayerControllerComponent.h"

#include "GameObject.h"
#include "InputManager.h"
#include "Renderer.h"
#include "utils.h"
#include "WalkCommand.h"

PlayerControllerComponent::PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int playerDims, glm::vec2 playerSize)
	: m_pLevelIndices{ pLevelIndices }
	, m_Gravity{0, 9}
	, m_Velocity{0, 0}
	, m_PlayerSize{ playerSize }
	, m_PlayerDims{playerDims}
	, m_MovementSpeed{ 50 }
{
}

void PlayerControllerComponent::UpdateWalking(MoveDirections dir)
{
	switch (dir)
	{
	case MoveDirections::Left:
		m_Velocity.x = -m_MovementSpeed;
		break;
	case MoveDirections::Right:
		m_Velocity.x = m_MovementSpeed;
		break;
	}
}

void PlayerControllerComponent::Startup()
{
	AddInput();
	m_pOwner->GetTransform().SetPosition( 0, -10, 0 );
}

void PlayerControllerComponent::Update(float deltaSec)
{
	UpdateGravity(deltaSec);
	HitFloor();
	TranslateSprite(deltaSec);
	m_Velocity.x = 0.0f;
}

void PlayerControllerComponent::UpdateGravity(float deltaTime)
{
	m_Velocity += m_Gravity * deltaTime;
}

void PlayerControllerComponent::TranslateSprite(float deltaTime) const
{
	m_pOwner->GetTransform().MoveWith(glm::vec3{ m_Velocity.x * deltaTime, m_Velocity.y * deltaTime, 0 });
}

bool PlayerControllerComponent::HitFloor()
{
	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	const glm::vec2 startP{ box.left + box.width / 2, box.bottom};
	const glm::vec2 endP{ box.left + box.width / 2, box.bottom + 1 };

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			m_Velocity.y = 0.0f;
			return true;
		}
	}
	return false;
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
	input.SetCommandToKey(0, SDLK_a, new WalkCommand(this, MoveDirections::Left), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_d, new WalkCommand(this, MoveDirections::Right), dae::InputManager::InputState::Hold);
}
