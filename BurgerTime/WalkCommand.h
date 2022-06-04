#pragma once
#include "Command.h"
#include "PlayerControllerComponent.h"

class WalkCommand final : public dae::Command
{
public:
	WalkCommand(PlayerControllerComponent* playerController, MoveDirections dir);
	~WalkCommand() override = default;
	void Execute() override;

private:
	PlayerControllerComponent* m_PlayerController{};
	MoveDirections m_Direction{};
};

