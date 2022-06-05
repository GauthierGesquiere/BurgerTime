#include "ThrowCommand.h"

ThrowCommand::ThrowCommand(PlayerControllerComponent* playerController)
	: m_PlayerController{ playerController }
{
}

void ThrowCommand::Execute()
{
	m_PlayerController->ThrowSalt();
}
