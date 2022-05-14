#pragma once
#include <memory>
#include <unordered_map>

#include "Command.h"
#include "Singleton.h"
#include "XboxController.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class InputType
		{
			KeyPressed,
			KeyUp,
			KeyDown,
			Idle
		};

		InputManager();
		InputManager(int controllerIndex);
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;
		~InputManager() override;

		bool ProcessInput();

		void SetCommandToButton(unsigned int controllerIndex, XboxController::ControllerButton button, Command* command, InputType inputType);

		void AddPLayer(unsigned int i = -1);

	private:
		std::unique_ptr<XboxController>* m_pXboxController = nullptr;
		std::vector<XboxController*> m_pControllers;

		struct KeyInfo
		{
			KeyInfo(unsigned int i, XboxController::ControllerButton button)
			: ControllerIndex{ i }
			, Button{ button }{}
			
			unsigned int ControllerIndex;
			XboxController::ControllerButton Button;

			bool operator==(const KeyInfo& other) const
			{
				return ControllerIndex == other.ControllerIndex && Button == other.Button;
			}
		};
		struct CommandInfo
		{
			CommandInfo(Command* pCommand, InputType inputType)
				: pCommand{ pCommand }
				, InputType{ inputType }{}

			Command* pCommand;
			InputType InputType;
		};
		struct KeyInfoHasher
		{
			size_t operator()(const KeyInfo& keyInfo) const
			{
				return reinterpret_cast<size_t>(&keyInfo);
			}
		};

		std::unordered_map<KeyInfo, CommandInfo, KeyInfoHasher> m_Commands;

	};

}
