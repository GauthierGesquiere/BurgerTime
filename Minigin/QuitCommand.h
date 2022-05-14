#pragma once
#include "Command.h"

namespace dae
{
	class QuitCommand final : public Command
	{
	public:
		QuitCommand() = default;
		~QuitCommand() = default;
		void Execute();
	};
}

