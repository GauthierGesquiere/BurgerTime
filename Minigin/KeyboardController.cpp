#include "MiniginPCH.h"
#include "KeyboardController.h"

#include <future>
#include <thread>


class dae::KeyboardController::KeyboardControllerImpl
{
private:
	std::vector<SDL_Keycode> m_FirstPressedKeys;
	//std::vector<SDL_Keycode> m_SecondPressedKeys;
	//Switch between the first and second pressed keys,
	//so you can check if the key is released, or is down
	//bool m_FirstKeysIsCurrent = false;

	SDL_Keycode m_PreviousKey{};
	SDL_Keycode m_CurrentKey{};

	WORD ButtonsPressedThisFrame{};
	WORD ButtonsReleasedThisFrame{};

	int m_ControllerIndex;

	int test{ 0 };

	SDL_Event e{};

public:

	KeyboardControllerImpl(int controllerIndex)
		: m_ControllerIndex{ controllerIndex }
	{
		SDL_PollEvent(&e);
		/*auto RunPollEventLoop = [this]()
		{
			SDL_Event e;
			while (this) 
			{
				m_PreviousKey = m_CurrentKey;
				m_CurrentKey = NULL;

				while (SDL_PollEvent(&e))
				{
					if (e.type == SDL_KEYDOWN)
					{
						m_CurrentKey = e.key.keysym.sym;
					}
					if (e.type == SDL_KEYUP)
					{
						m_CurrentKey = e.key.keysym.sym;
					}
				}

				const auto buttonChanges = m_CurrentKey ^ m_PreviousKey;
				ButtonsPressedThisFrame = buttonChanges & static_cast<WORD>(m_CurrentKey);
				ButtonsReleasedThisFrame = buttonChanges & ~(static_cast<WORD>(m_CurrentKey));
			}
		};*/

		//std::thread KeyboardInput(RunPollEventLoop);
		//KeyboardInput.detach();

		m_PreviousKey = NULL;
		m_CurrentKey = NULL;
	}

	void ProcessInput()
	{
		m_PreviousKey = m_CurrentKey;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
			{
				m_CurrentKey = e.key.keysym.sym;
			}
			if (e.type == SDL_KEYUP)
			{
				m_CurrentKey = NULL;
			}
		}

		const auto buttonChanges = m_CurrentKey ^ m_PreviousKey;
		ButtonsPressedThisFrame = buttonChanges & static_cast<WORD>(m_CurrentKey);
		ButtonsReleasedThisFrame = buttonChanges & ~(static_cast<WORD>(m_CurrentKey));
	}

	bool IsPressed(SDL_Keycode key) const
	{
		return static_cast<WORD>(m_CurrentKey) & int(key);
	};
	bool IsDown(SDL_Keycode key) const
	{
		return ButtonsPressedThisFrame & int(key);
	};
	bool IsUp(SDL_Keycode key) const
	{
		return ButtonsReleasedThisFrame & int(key);
	}
};


void dae::KeyboardController::ProcessInput()
{
	pImpl->ProcessInput();
}

bool dae::KeyboardController::IsPressed(SDL_Keycode key) const
{
	return pImpl->IsPressed(key);
}

bool dae::KeyboardController::IsDown(SDL_Keycode key) const
{
	// todo: return whether the given button is pressed or not.
	return pImpl->IsDown(key);
}

bool dae::KeyboardController::IsUp(SDL_Keycode key) const
{
	// todo: return whether the given button is pressed or not.	
	return pImpl->IsUp(key);
}

dae::KeyboardController::KeyboardController(unsigned int controllerIndex)
{
	pImpl = std::make_unique<KeyboardControllerImpl>(controllerIndex);
}

dae::KeyboardController::~KeyboardController()
{
	pImpl.reset();
}

dae::KeyboardController::KeyboardController()
{
	pImpl = std::make_unique<KeyboardControllerImpl>(0);
}

