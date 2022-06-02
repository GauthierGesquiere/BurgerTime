#pragma once
#include "Game.h"
struct SDL_Window;
namespace dae
{
	class Minigin
	{
	public:
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();

		Game* m_pGame{};

		unsigned int m_WindowWidth{ 636 };
		unsigned int m_WindowHeight{ 600 };

	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		const float FixedTimeStep = 0.02f;
		SDL_Window* m_Window{};
	};
}