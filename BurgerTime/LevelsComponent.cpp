#include "LevelsComponent.h"
#include <memory>

#include "BurgerComponent.h"
#include "DataComponent.h"
#include "EnemyControllerComponent.h"
#include "EnemyStateComponent.h"
#include "EventQueue.h"
#include "GameObject.h"
#include "LevelDataComponent.h"
#include "PlayerControllerComponent.h"
#include "PlayerDataComponent.h"
#include "PlayerStateComponent.h"
#include "RenderSpriteComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"


LevelsComponent::LevelsComponent(unsigned int width, unsigned int height, unsigned int level)
	: m_level{level}
	, m_SourcePath{"BurgerTime/"}
	, m_MaxAmountOfEnemies{ 5 }
	, m_BurgersInitialized{ true }
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	LoadLevel(m_level);
	CreatePlayers(1);

	LoadData();
}

LevelsComponent::~LevelsComponent()
{
}

bool LevelsComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "RestartLevel")
	{
		m_NeedsRestart = true;
	}
	if (event->Message == "GameOver")
	{
		m_GameOver = true;
	}
	if (event->Message == "FinishedBurger")
	{
		m_BurgersFinished++;
	}
	return false;
}

void LevelsComponent::Startup()
{
	Component::Startup();

	dae::EventQueue::GetInstance().Subscribe("RestartLevel", this);
	dae::EventQueue::GetInstance().Subscribe("GameOver", this);
	dae::EventQueue::GetInstance().Subscribe("FinishedBurger", this);
}

void LevelsComponent::Update(float deltaSec)
{
	if (m_BurgersFinished == static_cast<int>(m_pBurgers.size()))
	{
		//Go to next level
		
	}

	if (m_EnemiesInitialized != static_cast<int>(m_pEnemies.size()))
	{
		m_EnemiesInitialized = 0;
		m_pEnemyTransforms.clear();
		for (const auto& enemy : m_pEnemies)
		{
			if (enemy->GetComponentOfType<EnemyControllerComponent>()->m_IsInitialized)
			{
				m_pEnemyTransforms.push_back(&enemy->GetTransform());
				m_EnemiesInitialized++;
				m_BurgersInitialized = false;
			}
		}		
	}
	else if (!m_BurgersInitialized)
	{
		for (const auto& burger : m_pBurgers)
		{
			burger->GetComponentOfType<BurgerComponent>()->SetAllEnemies(m_pEnemies);
		}
		m_BurgersInitialized = true;
	}

	if (m_NeedsRestart || m_GameOver)
	{
		m_ElapsedSec += deltaSec;
		if (m_ElapsedSec > 3)
		{
			m_NeedsRestart = false;
			m_ElapsedSec = 0.0f;

			for (auto element : m_pEnemies)
			{
				dae::SceneManager::GetInstance().GetActiveScene()->Remove(element);
			}
			m_pEnemies.clear();

			dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_pPlayer);

			CreatePlayers(1);
			CreateEnemy();

			m_LevelIsReset = true;
			dae::EventQueue::GetInstance().Broadcast(new dae::Event("LevelIsReset"));

			if (m_GameOver)
			{
				std::string fullPath{ "Levels/GameOver.png" };

				const auto gObject = std::make_shared<dae::GameObject>();
				gObject->AddComponent(new RenderSpriteComponent(m_SourcePath + fullPath, m_LevelWidth, m_LevelHeight, m_WindowWidth, m_WindowHeight));
				dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
			}
		}
	}

	m_AmountOfEnemies = static_cast<int>(m_pEnemies.size());
	if (!m_GameOver)
	{
		if (m_AmountOfEnemies < m_MaxAmountOfEnemies)
		{
			if (rand() % 5 >= 2)
			{
				m_ElapsedSecEnemy += deltaSec;
			}
			if (m_ElapsedSecEnemy > 4)
			{
				m_ElapsedSecEnemy = 0.0f;
				CreateEnemy();
				for (const auto& enemy : m_pEnemies)
				{
					enemy->GetComponentOfType<EnemyControllerComponent>()->SetPlayerTransform(&m_pPlayer->GetTransform());
				}
				for (const auto& burger : m_pBurgers)
				{
					burger->GetComponentOfType<BurgerComponent>()->SetPlayerTransform(&m_pPlayer->GetTransform());
				}
			}
		}

		if (m_LevelIsReset)
		{
			for (const auto& enemy : m_pEnemies)
			{
				enemy->GetComponentOfType<EnemyControllerComponent>()->SetPlayerTransform(&m_pPlayer->GetTransform());
				m_LevelIsReset = false;
			}
		}
	}
}

void LevelsComponent::LoadLevel(unsigned levelIndex)
{
	//Get the image
	std::string fullPath{ "Levels/Level" };
	fullPath += std::to_string(levelIndex) + ".png";

	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent(m_SourcePath + fullPath, m_LevelWidth, m_LevelHeight, m_WindowWidth, m_WindowHeight));
	m_SourceToDestRatio = gObject->GetComponentOfType<RenderSpriteComponent>()->GetSourceToDestRatio();
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);

	//Parse the svg file
	fullPath = "Levels/Level" + std::to_string(levelIndex) + ".svg";
	dae::ResourceManager::GetInstance().GetVerticesFromSvgFile(m_SourcePath + fullPath, m_LevelVertices, m_SourceToDestRatio);

	MakeBurger(levelIndex);
}

void LevelsComponent::CreatePlayers(unsigned amount)
{
	for (unsigned int i = 0; i < amount; ++i)
	{
		const auto gObject = std::make_shared<dae::GameObject>();
		gObject->AddComponent(new RenderSpriteComponent());
		gObject->AddComponent(new PlayerStateComponent(m_WindowWidth, m_WindowHeight, m_PlayerDims, m_SourceToDestRatio));
		gObject->AddComponent(new PlayerControllerComponent(&m_LevelVertices, m_PlayerDims, m_SourceToDestRatio));
		dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
		m_pPlayer = gObject;
	}
}

void LevelsComponent::CreateEnemy()
{
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent());
	int randInt = rand() % 10;

	EnemyType type;
	if (randInt > 5)
	{
		type = EnemyType::Mr_HotDog;
	}
	else if (randInt > 2)
	{
		type = EnemyType::Mr_Egg;
	}
	else
	{
		type = EnemyType::Mr_Pickle;
	}
		//type = EnemyType::Mr_Egg;

	gObject->AddComponent(new EnemyStateComponent(m_WindowWidth, m_WindowHeight, m_PlayerDims, m_SourceToDestRatio, type));

	glm::vec2 spawnPoint{};
	randInt = rand() % 10;
	if (randInt > 6 || type == EnemyType::Mr_Egg)
	{
		spawnPoint = { 20, 423 };
	}
	else if (randInt > 4)
	{
		spawnPoint = { 570, 423 };
	}
	else if (randInt > 1)
	{
		spawnPoint = { 0, -10.f };
	}
	else if (randInt == 1)
	{
		spawnPoint = { 580, -10.f };
	}
	//spawnPoint = { 0, -10.f };
	gObject->AddComponent(new EnemyControllerComponent(&m_LevelVertices, m_PlayerDims, m_SourceToDestRatio, spawnPoint));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_pEnemies.push_back(gObject);
}

void LevelsComponent::LoadData()
{
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new PlayerDataComponent(m_SourceToDestRatio));
	gObject->AddComponent(new LevelDataComponent(m_SourceToDestRatio));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
}

void LevelsComponent::MakeBurger(int level)
{
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new BurgerComponent(level, 1, m_SourceToDestRatio, &m_LevelVertices));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_pBurgers.push_back(gObject);

	const auto gObject1 = std::make_shared<dae::GameObject>();
	gObject1->AddComponent(new BurgerComponent(level, 2, m_SourceToDestRatio, &m_LevelVertices));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject1);
	m_pBurgers.push_back(gObject1);

	const auto gObject2 = std::make_shared<dae::GameObject>();
	gObject2->AddComponent(new BurgerComponent(level, 3, m_SourceToDestRatio, &m_LevelVertices));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject2);
	m_pBurgers.push_back(gObject2);

	const auto gObject3 = std::make_shared<dae::GameObject>();
	gObject3->AddComponent(new BurgerComponent(level, 4, m_SourceToDestRatio, &m_LevelVertices));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject3);
	m_pBurgers.push_back(gObject3);
}
