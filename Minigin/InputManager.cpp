#include "MiniginPCH.h"
#include "InputManager.h"

dae::InputManager::InputManager()
{
	m_pControllers.push_back(new XboxController(0));
}

dae::InputManager::~InputManager()
{
	for (auto& controller : m_pControllers)
	{
		delete controller;
		controller = nullptr;
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
		m_pControllers[i]->ProcessInput();

		//For every command check what states it need, and then if check if its true, if so activate that command
		for (const auto& command : m_Commands)
		{
			switch (command.second.InputType)
			{
			case InputType::KeyPressed:
				if (m_pControllers[i]->IsPressed(command.first.Button))
				{
					command.second.pCommand->Execute();
				}
				break;
			case InputType::KeyUp:
				if (m_pControllers[i]->IsUp(command.first.Button))
				{
					command.second.pCommand->Execute();
				}
				break;
			case InputType::KeyDown:
				if (m_pControllers[i]->IsDown(command.first.Button))
				{
					command.second.pCommand->Execute();
				}
				break;
			default:
				break;
			}

			if (m_pControllers[i]->IsPressed(XboxController::ControllerButton::GAMEPAD_LEFT_SHOULDER))
			{
				return false;
			}
		}
	}
	return true;
}

void dae::InputManager::SetCommandToButton(unsigned int controllerIndex, XboxController::ControllerButton button, Command* command, InputType inputType)
{
	const KeyInfo keyInfo = { controllerIndex, button };
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
