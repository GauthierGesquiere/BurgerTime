#pragma once
#include "Command.h"

namespace dae
{
	class TestCommand final : public Command
	{
	public:
		TestCommand() = default;
		~TestCommand() = default;
		void Execute();
	};
}

