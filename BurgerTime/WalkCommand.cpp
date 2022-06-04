#include "WalkCommand.h"

WalkCommand::WalkCommand(PlayerControllerComponent* playerController, MoveDirections dir)
	: m_PlayerController{ playerController }
	, m_Direction{ dir }
{
}

void WalkCommand::Execute()
{
	m_PlayerController->UpdateWalking(m_Direction);
}
