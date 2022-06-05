#pragma once
#include "Event.h"

namespace dae
{
	class EventListener
	{
	public:
		EventListener() = default;
		virtual ~EventListener() = default;
		EventListener(const EventListener& other) = delete;
		EventListener(EventListener&& other) = delete;
		EventListener& operator=(const EventListener& other) = delete;
		EventListener& operator=(EventListener&& other) = delete;

		virtual bool OnEvent(const Event* event) = 0;
	};
}

