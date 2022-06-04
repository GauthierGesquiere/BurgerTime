#pragma once
#include <memory>

namespace dae
{
	enum class ControllerButton
	{
		GAMEPAD_DPAD_UP = 0x0001,
		GAMEPAD_DPAD_DOWN = 0x0002,
		GAMEPAD_DPAD_LEFT = 0x0004,
		GAMEPAD_DPAD_RIGHT = 0x0008,
		GAMEPAD_START = 0x0010,
		GAMEPAD_BACK = 0x0020,
		GAMEPAD_LEFT_THUMB = 0x0040,
		GAMEPAD_RIGHT_THUMB = 0x0080,
		GAMEPAD_LEFT_SHOULDER = 0x0100,
		GAMEPAD_RIGHT_SHOULDER = 0x0200,
		GAMEPAD_A = 0x1000,
		GAMEPAD_B = 0x2000,
		GAMEPAD_X = 0x4000,
		GAMEPAD_Y = 0x8000
	};

	class XboxController final
	{
	public:

		XboxController();
		XboxController(unsigned int controllerIndex);
		XboxController(const XboxController& other) = delete;
		XboxController(XboxController&& other) = delete;
		XboxController& operator=(const XboxController& other) = delete;
		XboxController& operator=(XboxController&& other) = delete;
		~XboxController();

		bool ProcessInput();
		bool IsHold(ControllerButton button) const;
		bool IsPressed(ControllerButton button) const;
		bool IsReleased(ControllerButton button) const;

	private:
		class XboxControllerImpl;
		std::unique_ptr<XboxControllerImpl> pImpl;
	};
};