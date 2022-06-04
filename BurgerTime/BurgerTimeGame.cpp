#include "BurgerTimeGame.h"

#include <SDL_keycode.h>

#include "GameObject.h"
#include "InputManager.h"
#include "LevelsComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TestCommand.h"
#include "WalkCommand.h"

BurgerTimeGame::BurgerTimeGame(unsigned int width, unsigned int height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;
}

void BurgerTimeGame::LoadGame()
{
	//Make Scene
	const std::string sceneName = "BurgerTime";
	dae::Scene& gameScene = dae::SceneManager::GetInstance().CreateScene(sceneName);
	dae::SceneManager::GetInstance().SetSceneAsActive(sceneName);

	const auto gObject = std::make_shared<dae::GameObject>();
	const auto levelsComponent = new LevelsComponent(m_WindowWidth, m_WindowHeight);
	gObject->AddComponent(levelsComponent);

	gameScene.Add(gObject);

	InitInput();
}

void BurgerTimeGame::InitInput()
{
	//auto& input = dae::InputManager::GetInstance();
	//input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_A, new dae::TestCommand(), dae::InputManager::InputState::Pressed);
	//input.SetCommandToKey(0, SDLK_q, new dae::TestCommand(), dae::InputManager::InputState::KeyDown);
	//input.SetCommandToKey(0, SDLK_w, new dae::TestCommand(), dae::InputManager::InputState::Pressed);
	//input.SetCommandToKey(0, SDLK_q, new dae::TestCommand(), dae::InputManager::InputState::Released);
	//input.SetCommandToKey(0, SDLK_s, new dae::TestCommand(), dae::InputManager::InputState::Hold);
}
