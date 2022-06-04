#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"

dae::InputManager::InputManager()
{
	m_pControllers.push_back(new XboxController(0));
	m_pKeyboards.push_back(new KeyboardController(0));
}

dae::InputManager::InputManager(int controllerIndex)
{
	m_pControllers.push_back(new XboxController(controllerIndex));
	m_pKeyboards.push_back(new KeyboardController(controllerIndex));
}

dae::InputManager::~InputManager()
{
	for (auto& controller : m_pControllers)
	{
		delete controller;
		controller = nullptr;
	}

	for (auto& keyboard : m_pKeyboards)
	{
		delete keyboard;
		keyboard = nullptr;
	}

	for (auto& command : m_Commands)
	{
		delete command.second.pCommand;
		command.second.pCommand = nullptr;
	}
}

bool dae::InputManager::ProcessInput()
{
	for (size_t i = 0; i < m_pControllers.size(); ++i)
	{
		//check input
		if (!m_pKeyboards[i]->ProcessInput())
		{
			return false;
		}

		m_pControllers[i]->ProcessInput();

		//For every command check what states it need, and then if check if its true, if so activate that command
		for (const auto& command : m_Commands)
		{
			switch (command.second.InputType)
			{
			case InputState::Hold:
				if (m_pControllers[i]->IsHold(command.first.Button))
				{
					command.second.pCommand->Execute();
				}
				else if(m_pKeyboards[i]->IsHold(command.first.Key))
				{
					command.second.pCommand->Execute();
				}
				break;
			case InputState::Released:
				if (m_pControllers[i]->IsReleased(command.first.Button))
				{
					command.second.pCommand->Execute();
				}
				else if (m_pKeyboards[i]->IsReleased(command.first.Key))
				{
					command.second.pCommand->Execute();
				}
				break;
			case InputState::Pressed:
				if (m_pControllers[i]->IsPressed(command.first.Button))
				{
					command.second.pCommand->Execute();
				}
				else if (m_pKeyboards[i]->IsPressed(command.first.Key))
				{
					command.second.pCommand->Execute();
				}
				break;
			default:
				break;
			}
		}
	}
	return true;
}

void dae::InputManager::SetCommandToButton(unsigned int controllerIndex, ControllerButton button, Command* command, InputState inputType)
{
	const KeyInfo keyInfo = { button,  controllerIndex};
	const CommandInfo commandInfo = { command, inputType };
	m_Commands.insert({ keyInfo, commandInfo });
}

void dae::InputManager::SetCommandToKey(unsigned int controllerIndex, SDL_Keycode key, Command* command, InputState inputType)
{
	const KeyInfo keyInfo = { key , controllerIndex};
	const CommandInfo commandInfo = { command, inputType };
	m_Commands.insert({ keyInfo, commandInfo });
}

void dae::InputManager::AddPLayer(unsigned int /*i*/)
{
	/*if (i == -1)
	{
		m_pControllers.push_back(new XboxController(unsigned int(m_pControllers.size())));
	}
	else
	{
		m_pControllers.push_back(new XboxController(i));
	}*/
}
