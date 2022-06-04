#include "MiniginPCH.h"
#include "XboxController.h"
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")


class dae::XboxController::XboxControllerImpl
{
private:
	XINPUT_STATE CurrentState{};
	XINPUT_STATE PreviousState{};

	WORD ButtonsPressedThisFrame{};
	WORD ButtonsReleasedThisFrame{};

	int m_ControllerIndex;
public:
	XboxControllerImpl(int controllerIndex)
		: m_ControllerIndex{ controllerIndex }
	{
		ZeroMemory(&PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&CurrentState, sizeof(XINPUT_STATE));
	}
	bool ProcessInput()
	{
		CopyMemory(&PreviousState, &CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &CurrentState);

		auto buttonChanges = CurrentState.Gamepad.wButtons ^ PreviousState.Gamepad.wButtons;
		ButtonsPressedThisFrame = buttonChanges & CurrentState.Gamepad.wButtons;
		ButtonsReleasedThisFrame = buttonChanges & ~CurrentState.Gamepad.wButtons;

		return true;
	}

	bool IsHold(ControllerButton button) const { return CurrentState.Gamepad.wButtons & int(button); }
	bool IsPressed(ControllerButton button) const { return ButtonsPressedThisFrame & int(button); }
	bool IsReleased(ControllerButton button) const { return ButtonsReleasedThisFrame & int(button); }
};


bool dae::XboxController::ProcessInput()
{
	return pImpl->ProcessInput();
}

bool dae::XboxController::IsHold(ControllerButton button) const
{
	return pImpl->IsHold(button);
}

bool dae::XboxController::IsPressed(ControllerButton button) const
{
	// todo: return whether the given button is pressed or not.
	return pImpl->IsPressed(button);
}

bool dae::XboxController::IsReleased(ControllerButton button) const
{
	// todo: return whether the given button is pressed or not.	
	return pImpl->IsReleased(button);
}

dae::XboxController::XboxController(unsigned int controllerIndex)
{
	pImpl = std::make_unique<XboxControllerImpl>(controllerIndex);
}

dae::XboxController::~XboxController()
{
	pImpl.reset();
}

dae::XboxController::XboxController()
{
	pImpl = std::make_unique<XboxControllerImpl>(0);
}

