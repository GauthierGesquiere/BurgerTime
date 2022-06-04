#include "MiniginPCH.h"
#include "KeyboardController.h"

#include <future>
#include <thread>


class dae::KeyboardController::KeyboardControllerImpl
{
private:
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

		m_PreviousKey = NULL;
		m_CurrentKey = NULL;
	}

	bool ProcessInput()
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
			if (e.type == SDL_QUIT) 
			{
				return false;
			}
		}

		const auto buttonChanges = static_cast<WORD>(m_CurrentKey) ^ static_cast<WORD>(m_PreviousKey);
		ButtonsPressedThisFrame = buttonChanges & static_cast<WORD>(m_CurrentKey);
		ButtonsReleasedThisFrame = buttonChanges & ~static_cast<WORD>(m_CurrentKey);
		
		return true;
	}

	bool IsHold(SDL_Keycode key)const
	{
		if (static_cast<WORD>(m_CurrentKey) == int(key) && m_CurrentKey != 0)
		{
			return true;
		}
		return false;

	}
	bool IsPressed(SDL_Keycode key) const
	{
		if (ButtonsPressedThisFrame == int(key) && ButtonsPressedThisFrame != 0)
		{
			return true;
		}
		return false;
	}
	bool IsReleased(SDL_Keycode key) const
	{
		if (ButtonsReleasedThisFrame == int(key) && ButtonsReleasedThisFrame != 0)
		{
			return true;
		}
		return false;
	}
};

bool dae::KeyboardController::ProcessInput()
{
	return pImpl->ProcessInput();
}

bool dae::KeyboardController::IsHold(SDL_Keycode key) const
{
	return pImpl->IsHold(key);
}

bool dae::KeyboardController::IsPressed(SDL_Keycode key) const
{
	// todo: return whether the given button is pressed or not.
	return pImpl->IsPressed(key);
}

bool dae::KeyboardController::IsReleased(SDL_Keycode key) const
{
	// todo: return whether the given button is pressed or not.	
	return pImpl->IsReleased(key);
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

