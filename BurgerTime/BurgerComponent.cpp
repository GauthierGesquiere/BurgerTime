#include "BurgerComponent.h"

#include <memory>

#include "BurgerPieceComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"

BurgerComponent::BurgerComponent(int level, int column, glm::vec2 sourceToDestRatio, std::vector<std::vector<glm::vec2>>* pLevelIndices)
	: m_Level{level}
	  , m_Column{column}
	  , m_SourceToDestRatio{sourceToDestRatio}
	  , m_pLevelIndices{pLevelIndices}
{
}

void BurgerComponent::Startup()
{
	Component::Startup();
	MakeBurger();
	
}

void BurgerComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);

	for (int i = static_cast<int>(m_BurgerPieces.size() - 1); i >= 1; i--)
	{
		const auto piece1 = m_BurgerPieces[i]->GetComponentOfType<BurgerPieceComponent>();
		const auto piece2 = m_BurgerPieces[i - 1]->GetComponentOfType<BurgerPieceComponent>();
		if (piece1->m_IsInitialized && piece2->m_IsInitialized)
		{
			piece1->CheckPieceAbove(piece2);
		}
	}

	for (const auto& piece : m_BurgerPieces)
	{
		piece->GetComponentOfType<BurgerPieceComponent>()->CheckIfHitEnemy(m_pEnemies);
	}
}

void BurgerComponent::SetPlayerTransform(dae::Transform* pPlayerTransform)
{
	for (const auto burgerPiece : m_BurgerPieces)
	{
		burgerPiece->GetComponentOfType<BurgerPieceComponent>()->SetPlayerTransform(pPlayerTransform);
	}
}

void BurgerComponent::SetAllEnemies(std::vector<std::shared_ptr<dae::GameObject>>& pEnemies)
{
	m_pEnemies = pEnemies;
}

void BurgerComponent::MakeBurger()
{
	auto posY1 = 0.0f;
	auto posY2 = 0.0f;
	auto posY3 = 0.0f;
	auto posY4 = 0.0f;
	if (m_Column == 1)
	{
		m_PosX = 50.0f;
		posY1 = 20.0f;
		posY2 = 115.0f;
		posY3 = 215.0f;
		posY4 = 360.0f;
	}
	else if (m_Column == 2)
	{
		m_PosX = 200.0f;
		posY1 = 20.0f;
		posY2 = 165.0f;
		posY3 = 257.0f;
		posY4 = 360.0f;
	}
	else if (m_Column == 3)
	{
		m_PosX = 340.0f;
		posY1 = 20.0f;
		posY2 = 115.0f;
		posY3 = 257.0f;
		posY4 = 360.0f;
	}
	else if (m_Column == 4)
	{
		m_PosX = 490.0f;
		posY1 = 20.0f;
		posY2 = 115.0f;
		posY3 = 215.0f;
		posY4 = 310.0f;
	}

	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new BurgerPieceComponent(glm::vec2{ m_PosX, posY1 }, 5, BurgerPiece::TopBread, m_SourceToDestRatio, m_pLevelIndices));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_BurgerPieces.push_back(gObject);

	const auto gObject1 = std::make_shared<dae::GameObject>();
	gObject1->AddComponent(new BurgerPieceComponent(glm::vec2{ m_PosX, posY2 }, 4, BurgerPiece::Salad, m_SourceToDestRatio, m_pLevelIndices));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject1);
	m_BurgerPieces.push_back(gObject1);

	const auto gObject2 = std::make_shared<dae::GameObject>();
	gObject2->AddComponent(new BurgerPieceComponent(glm::vec2{ m_PosX, posY3 }, 3, BurgerPiece::Meat, m_SourceToDestRatio, m_pLevelIndices));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject2);
	m_BurgerPieces.push_back(gObject2);

	const auto gObject3 = std::make_shared<dae::GameObject>();
	gObject3->AddComponent(new BurgerPieceComponent(glm::vec2{ m_PosX, posY4 }, 2, BurgerPiece::BottomBread, m_SourceToDestRatio, m_pLevelIndices));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject3);
	m_BurgerPieces.push_back(gObject3);
}

void BurgerComponent::CheckIfHitEnemy()
{
	if (true)
	{
		
	}
}
