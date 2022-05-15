#pragma once
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

		void ProcessInput();
		bool IsPressed(SDL_Keycode key) const;
		bool IsDown(SDL_Keycode key) const;
		bool IsUp(SDL_Keycode key) const;

	private:
		class KeyboardControllerImpl;
		std::unique_ptr<KeyboardControllerImpl> pImpl;
	};
};

