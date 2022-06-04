#pragma once
#include <memory>
#include <SDL.h>

namespace dae
{
	class KeyboardController final
	{
	public:
		KeyboardController();
		KeyboardController(unsigned int controllerIndex);
		KeyboardController(const KeyboardController& other) = delete;
		KeyboardController(KeyboardController&& other) = delete;
		KeyboardController& operator=(const KeyboardController& other) = delete;
		KeyboardController& operator=(KeyboardController&& other) = delete;
		~KeyboardController();

		bool ProcessInput();
		bool IsHold(SDL_Keycode key) const;
		bool IsPressed(SDL_Keycode key) const;
		bool IsReleased(SDL_Keycode key) const;

	private:
		class KeyboardControllerImpl;
		std::unique_ptr<KeyboardControllerImpl> pImpl;
	};
};

