#include "MiniginPCH.h"
#include "InputManager.h"
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")


class dae::InputManager::ControllerImpl
{
	XINPUT_STATE CurrentState{};
	XINPUT_STATE PreviousState{};

	WORD ButtonsPressedThisFrame{};
	WORD ButtonsReleasedThisFrame{};

	int ControllerIndex;
public:
	ControllerImpl(int /*controllerIndex*/)
	{
		ZeroMemory(&PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&CurrentState, sizeof(XINPUT_STATE));
	}
	void ProcessInput()
	{
		CopyMemory(&PreviousState, &CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(0, &CurrentState);

		auto buttonChanges = CurrentState.Gamepad.wButtons ^ PreviousState.Gamepad.wButtons;
		ButtonsPressedThisFrame = buttonChanges & CurrentState.Gamepad.wButtons;
		ButtonsReleasedThisFrame = buttonChanges & (~CurrentState.Gamepad.wButtons);
	}

	bool IsPressed(ControllerButton button) const { return CurrentState.Gamepad.wButtons & int(button); };
	bool IsDown(ControllerButton button) const { return ButtonsPressedThisFrame & int(button); };
	bool IsUp(ControllerButton button) const { return ButtonsReleasedThisFrame & int(button); };
};


void dae::InputManager::ProcessInput()
{
	pImpl->ProcessInput();
}

bool dae::InputManager::IsPressed(ControllerButton button) const
{
	return pImpl->IsPressed(button);
}

bool dae::InputManager::IsDown(ControllerButton button) const
{
	// todo: return whether the given button is pressed or not.
	return pImpl->IsDown(button);
}

bool dae::InputManager::IsUp(ControllerButton button) const
{
	// todo: return whether the given button is pressed or not.	
	return pImpl->IsUp(button);
}

dae::InputManager::InputManager(int controllerIndex)
{
	pImpl = std::make_unique<ControllerImpl>(controllerIndex);
}

dae::InputManager::~InputManager()
{
	pImpl.reset();
}

dae::InputManager::InputManager()
{
	pImpl = std::make_unique<ControllerImpl>(0);
}
