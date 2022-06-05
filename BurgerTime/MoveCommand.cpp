#include "MoveCommand.h"

MoveCommand::MoveCommand(PlayerControllerComponent* playerController, MoveDirections dir)
	: m_PlayerController{ playerController }
	, m_Direction{ dir }
{
}

void MoveCommand::Execute()
{
	m_PlayerController->UpdateMovement(m_Direction);
}
