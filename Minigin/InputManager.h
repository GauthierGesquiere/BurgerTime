#pragma once
#include <unordered_map>

#include "Command.h"
#include "KeyboardController.h"
#include "Singleton.h"
#include "XboxController.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class InputState
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

		void SetCommandToButton(unsigned int controllerIndex, ControllerButton button, Command* command, InputState inputType);
		void SetCommandToKey(unsigned int controllerIndex, SDL_Keycode key, Command* command, InputState inputType);

		void AddPLayer(unsigned int i = -1);

	private:
		std::vector<XboxController*> m_pControllers;
		std::vector<KeyboardController*> m_pKeyboards;

		enum class InputType
		{
			XboxController,
			KeyBoard
		};

		struct KeyInfo
		{
			KeyInfo(unsigned int i, ControllerButton button)
			: PlayerControllerIndex{ i }
			, Button{ button }
			{
				Key = 0;
				type = InputType::XboxController;
			}
			KeyInfo(unsigned int i, SDL_Keycode key)
				: PlayerControllerIndex{ i }
				, Key{ key }
			{
				type = InputType::KeyBoard;
			}
			
			unsigned int PlayerControllerIndex;

			ControllerButton Button{};
			SDL_Keycode Key{};

			InputType type;

			bool operator==(const KeyInfo& other) const
			{
				return PlayerControllerIndex == other.PlayerControllerIndex && Button == other.Button;
			}
		};
		struct CommandInfo
		{
			CommandInfo(Command* pCommand, InputState inputType)
				: pCommand{ pCommand }
				, InputType{ inputType }{}

			Command* pCommand;
			InputState InputType;
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
