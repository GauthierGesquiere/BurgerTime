#pragma once
#include "Command.h"
#include "PlayerControllerComponent.h"

class ThrowCommand : public dae::Command
{
public:
	ThrowCommand(PlayerControllerComponent* playerController);
	~ThrowCommand() override = default;
	void Execute() override;

private:
	PlayerControllerComponent* m_PlayerController{};
};

