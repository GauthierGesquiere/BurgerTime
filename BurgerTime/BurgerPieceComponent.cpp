#include "BurgerPieceComponent.h"

#include <iostream>

#include "EventQueue.h"
#include "GameObject.h"
#include "RenderSpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"

BurgerPieceComponent::BurgerPieceComponent(glm::vec2 pos, unsigned amountOfDrops, BurgerPiece piece, glm::vec2 sourceToDestRatio, std::vector<std::vector<glm::vec2>>* pLevelIndices)
	: m_AmountOfDrops{amountOfDrops}
	  , m_Position{pos}
	  , m_Piece{ piece }
	  , m_SourcePathg{"BurgerTime/Burger/"}
	  , m_SourceDims{31, 8}
	  , m_SourceToDestRatio{sourceToDestRatio}
	  , m_isFirstUpdate{ true }
	  , m_Gravity{ 0, 35 }
	  , m_Velocity{ 0, 0 }
	  , m_pLevelIndices{pLevelIndices}
{
}

void BurgerPieceComponent::Startup()
{
	Component::Startup();

	std::string fullPath;

	switch (m_Piece)
	{
	case BurgerPiece::TopBread:
		fullPath = "BurgerBread.png";
		m_FreezeFrame = 0;
		break;
	case BurgerPiece::Tomato: 
		fullPath = "Salad_Tomato.png";
		m_FreezeFrame = 0;
		break;
	case BurgerPiece::Salad: 
		fullPath = "Salad_Tomato.png";
		m_FreezeFrame = 1;
		break;
	case BurgerPiece::Meat:
		fullPath = "Meat_Cheese.png";
		m_FreezeFrame = 1;
		break;
	case BurgerPiece::Cheese: 
		fullPath = "Meat_Cheese.png";
		m_FreezeFrame = 0;
		break;
	case BurgerPiece::BottomBread: 
		fullPath = "BurgerBread.png";
		m_FreezeFrame = 1;
		break;
	}

	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(
		new RenderSpriteComponent(m_SourcePathg + fullPath, static_cast<unsigned int>(m_SourceDims.x), static_cast<unsigned int>(m_SourceDims.y),
			static_cast<unsigned int>(m_SourceDims.x * m_SourceToDestRatio.x),
			static_cast<unsigned int>(m_SourceDims.y * m_SourceToDestRatio.y),
                                    LoopType::NoLoop));
	gObject->GetTransform().SetPosition(m_Position.x, m_Position.y, 0);
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_PieceObj = gObject;
}

void BurgerPieceComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);

	if (m_isFirstUpdate)
	{
		m_PieceObj->GetComponentOfType<RenderSpriteComponent>()->SetFreeze(m_FreezeFrame);
		m_isFirstUpdate = false;
	}

	CheckIfPlayerCoveredPiece(deltaSec);

	if (m_AmountOfDrops == 0)
	{
		m_NeedsToStack = true;
	}
}

void BurgerPieceComponent::SetPlayerTransform(dae::Transform* pPlayerTransform)
{
	m_pPlayerTransform = pPlayerTransform;
	m_IsInitialized = true;
}

void BurgerPieceComponent::UpdateGravity(float deltaTime)
{
	m_Velocity += m_Gravity * deltaTime;
	m_PieceObj->GetTransform().MoveWith(glm::vec3{ m_Velocity.x * deltaTime, m_Velocity.y * deltaTime, 0 });
}

bool BurgerPieceComponent::CheckPieceAbove(BurgerPieceComponent* other)
{
	const utils::Rectf box = CalculateBox();
	const utils::Rectf otherBox = other->CalculateBox();

	if (IsOverlapping(box, otherBox))
	{
		if (m_NeedsToStack)
		{
			if ((other->m_Piece == BurgerPiece::TopBread) && !BroadcastOnce)
			{
				dae::EventQueue::GetInstance().Broadcast(new dae::Event("FinishedBurger"));
				BroadcastOnce = true;
			}
			other->m_NeedsToStack = true;
			other->m_Velocity.y = 0;
			return true;
		}

		m_PieceFellOn = true;
		return true;
	}
	return false;
}

void BurgerPieceComponent::CheckIfHitEnemy(std::vector<std::shared_ptr<dae::GameObject>>& pEnemies)
{
	if (!m_IsFalling)
	{
		return;
	}

	//std::shared_ptr<dae::GameObject> deletedEnemy;
	for (auto enemy : pEnemies)
	{
		auto enemyBox = enemy->GetTransform().GetRect();
		enemyBox.left = enemy->GetTransform().GetPosition().x;
		enemyBox.bottom = enemy->GetTransform().GetPosition().y;
		enemyBox.width /= 2;
		enemyBox.height /= 2;

		const auto box = CalculateBox();

		if (IsOverlapping(enemyBox, box))
		{
			dae::SceneManager::GetInstance().GetActiveScene()->Remove(enemy);
		}
	}
}

void BurgerPieceComponent::CheckIfPlayerCoveredPiece(float deltaSec)
{
	if (!m_pPlayerTransform)
	{
		return;
	}

	auto playerRect = m_pPlayerTransform->GetRect();

	playerRect.left = m_pPlayerTransform->GetPosition().x;
	playerRect.bottom = m_pPlayerTransform->GetPosition().y + playerRect.height;

	if (!m_HasRightSide)
	{
		//const auto topRighttOfPiece = glm::vec2{ m_PieceObj->GetTransform().GetPosition().x + m_SourceDims.x * m_SourceToDestRatio.x, m_PieceObj->GetTransform().GetPosition().y };
		const auto bottomRighttOfPiece = glm::vec2{ m_PieceObj->GetTransform().GetPosition().x + m_SourceDims.x * m_SourceToDestRatio.x, m_PieceObj->GetTransform().GetPosition().y + m_SourceDims.y * m_SourceToDestRatio.y };


		if (IsPointInRect(bottomRighttOfPiece, playerRect))// (playerRect, topRighttOfPiece, bottomRighttOfPiece, intersectMin, intersectMax))
		{
			m_HasRightSide = true;
		}
	}

	if (!m_HasLeftSide)
	{
		//const auto topLeftOfPiece = glm::vec2{ m_PieceObj->GetTransform().GetPosition().x, m_PieceObj->GetTransform().GetPosition().y };
		const auto bottomLeftOfPiece = glm::vec2{ m_PieceObj->GetTransform().GetPosition().x, m_PieceObj->GetTransform().GetPosition().y + m_SourceDims.y * m_SourceToDestRatio.y };

		//if (IntersectRectLine(playerRect, topLeftOfPiece, bottomLeftOfPiece, intersectMin, intersectMax))
		if (IsPointInRect(bottomLeftOfPiece, playerRect))
		{
			m_HasLeftSide = true;
		}
	}

	if (!m_HasMid)
	{
		//const auto topMidOfPiece = glm::vec2{ m_PieceObj->GetTransform().GetPosition().x + (m_SourceDims.x * m_SourceToDestRatio.x) / 2, m_PieceObj->GetTransform().GetPosition().y };
		const auto bottomMidOfPiece = glm::vec2{ m_PieceObj->GetTransform().GetPosition().x + (m_SourceDims.x * m_SourceToDestRatio.x) / 2, m_PieceObj->GetTransform().GetPosition().y + m_SourceDims.y * m_SourceToDestRatio.y };

		//if (IntersectRectLine(playerRect, topMidOfPiece, bottomMidOfPiece, intersectMin, intersectMax))
		if (IsPointInRect(bottomMidOfPiece, playerRect))
		{
			m_HasMid = true;
		}
	}

	if (m_HasRightSide && m_HasLeftSide && m_HasMid || m_PieceFellOn)
	{
		m_IsFalling = true;
		if (!m_NeedsToStack)
		{
			UpdateGravity(deltaSec);
		}
		ElapsedSec += deltaSec;

		HitFloor();
	}
	else
	{
		m_Velocity.y = 0.0f;
	}
}

const utils::Rectf BurgerPieceComponent::CalculateBox(glm::vec2 posToCalc) const
{
	if (posToCalc == glm::vec2{ -1, -1 })
	{
		utils::Rectf box;
		const glm::vec2 topLeft = m_PieceObj->GetTransform().GetPosition();
		box.left = topLeft.x;
		box.bottom = topLeft.y + m_SourceDims.y * m_SourceToDestRatio.y;
		box.width = m_SourceDims.x * m_SourceToDestRatio.x;
		box.height = m_SourceDims.y * m_SourceToDestRatio.y;
		return box;
	}

	utils::Rectf box;
	const glm::vec2 topLeft = posToCalc;
	box.left = topLeft.x;
	box.bottom = topLeft.y + m_SourceDims.y * m_SourceToDestRatio.y;
	box.width = m_SourceDims.x * m_SourceToDestRatio.x;
	box.height = m_SourceDims.y * m_SourceToDestRatio.y;
	return box;
}

bool BurgerPieceComponent::HitFloor()
{
	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	const glm::vec2 startP{ box.left + box.width / 2, box.bottom - 3 };
	const glm::vec2 endP{ box.left + box.width / 2, box.bottom };

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
			if (ElapsedSec >= 0.5f)
			{
				ElapsedSec = 0.0f;
				m_HasRightSide = false;
				m_HasLeftSide = false;
				m_HasMid = false;
				m_PieceFellOn = false;
				m_AmountOfDrops--;
				m_IsFalling = false;
				return true;
			}
		}
	}
	return false;
}
