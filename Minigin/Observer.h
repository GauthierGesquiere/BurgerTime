#pragma once
#include "Event.h"
namespace dae
{
	class GameObject;

	class Observer
	{
	public:
		virtual ~Observer();
		virtual void OnNotify(const GameObject& actor, Event* event) = 0;
	};
}
