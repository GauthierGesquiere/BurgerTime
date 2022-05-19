#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>

#include "EventQueue.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"
#include "TestCommand.h"

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	// input
	auto& input = InputManager::GetInstance();
	input.SetCommandToButton(0, XboxController::ControllerButton::GAMEPAD_A, new TestCommand(), InputManager::InputState::KeyDown);
	input.SetCommandToKey(0, SDLK_q, new TestCommand(), InputManager::InputState::KeyDown);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();
	LoadGame();
	

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& eventQueue = EventQueue::GetInstance();


		bool doContinue = true;
		float lag = 0.0f;
		auto lastTime = std::chrono::high_resolution_clock::now();

		//float test = 0.0f;



		while (doContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;

			doContinue = input.ProcessInput();

			/*test += deltaTime;
			if (test > 3)
			{
				doContinue = false;
			}*/

			while (lag >= FixedTimeStep)
			{
				sceneManager.Update(FixedTimeStep);
				lag -= FixedTimeStep;
			}
			sceneManager.Update(deltaTime);

			eventQueue.HandleEventQueue();
			sceneManager.GetActiveScene()->DeleteRemovedObjects();
			sceneManager.GetActiveScene()->AddObjects();
			renderer.Render();
		}
	}

	Cleanup();
}
