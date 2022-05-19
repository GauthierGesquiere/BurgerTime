#pragma once
#include "Event.h"
namespace dae
{
	class GameObject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const GameObject& actor, Event event) = 0;
	};
}
