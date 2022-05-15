#include "MiniginPCH.h"
#include "KeyboardController.h"



class dae::KeyboardController::KeyboardControllerImpl
{
private:
	std::vector<SDL_Keycode> m_FirstPressedKeys;
	std::vector<SDL_Keycode> m_SecondPressedKeys;
	//Switch between the first and second pressed keys,
	//so you can check if the key is released, or is down
	bool m_FirstKeysIsCurrent = false;

	SDL_Keycode m_PreviousKey{};
	SDL_Keycode m_CurrentKey{};

	int m_ControllerIndex;
public:
	KeyboardControllerImpl(int controllerIndex)
		: m_ControllerIndex{ controllerIndex }
	{}

	void ProcessInput()
	{
		m_PreviousKey = m_CurrentKey;

		m_FirstKeysIsCurrent = !m_FirstKeysIsCurrent;
		if (m_FirstKeysIsCurrent)
		{
			m_FirstPressedKeys.clear();
		}
		else
		{
			m_SecondPressedKeys.clear();
		}


		m_CurrentKey = NULL;

		SDL_Event e;
		while (SDL_PollEvent(&e)) 
		{
			if (e.type == SDL_KEYDOWN)
			{
				if (m_FirstKeysIsCurrent)
				{
					m_FirstPressedKeys.push_back(e.key.keysym.sym);
					m_CurrentKey = e.key.keysym.sym;
				}
				else
				{
					m_SecondPressedKeys.push_back(e.key.keysym.sym);
					m_CurrentKey = e.key.keysym.sym;
				}
			}
			
		}
	}

	bool IsPressed(SDL_Keycode key) const
	{
		if (m_FirstKeysIsCurrent)
		{
			return CheckKeys(key, m_FirstPressedKeys);
		}
		return CheckKeys(key, m_SecondPressedKeys);
	};
	bool IsDown(SDL_Keycode key) const
	{
		if (m_FirstKeysIsCurrent)
		{
			return CheckKeys(key, m_SecondPressedKeys);
		}
		return CheckKeys(key, m_FirstPressedKeys);
	};
	bool IsUp(SDL_Keycode key) const
	{
		if (m_CurrentKey != m_PreviousKey)
		{
			if (m_FirstKeysIsCurrent)
			{
				return CheckKeys(key, m_FirstPressedKeys);
			}
			return CheckKeys(key, m_SecondPressedKeys);
		}
		return false;
	}

	bool CheckKeys(const SDL_Keycode& keycode, const std::vector<SDL_Keycode>& keysToCheck) const
	{
		const auto findItr = std::find(keysToCheck.begin(), keysToCheck.end(), keycode);
		return findItr != keysToCheck.end();
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

