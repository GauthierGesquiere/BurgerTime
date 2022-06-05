#pragma once
#include "Command.h"
#include "PlayerControllerComponent.h"

class MoveCommand final : public dae::Command
{
public:
	MoveCommand(PlayerControllerComponent* playerController, MoveDirections dir);
	~MoveCommand() override = default;
	void Execute() override;

private:
	PlayerControllerComponent* m_PlayerController{};
	MoveDirections m_Direction{};
};

