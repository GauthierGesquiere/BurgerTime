#pragma once
#include <map>
#include <queue>
#include <unordered_map>

#include "Event.h"
#include "EventListener.h"
#include "Singleton.h"

namespace dae
{
	class EventQueue final : public Singleton<EventQueue>
	{
	public:
		EventQueue() = default;
		~EventQueue();
		EventQueue(const EventQueue& other) = delete;
		EventQueue(EventQueue&& other) = delete;
		EventQueue& operator=(const EventQueue& other) = delete;
		EventQueue& operator=(EventQueue&& other) = delete;

		void Broadcast(Event* event); // is sent to all listeners

		void Subscribe(const std::string& eventType, EventListener* pListener);
		//For cleanup
		void Unsubscribe(EventListener* pListener);
		void Unsubscribe(const std::string& eventType, EventListener* pListener);

		void HandleEventQueue();

	private:
		std::queue<Event*> m_EventQue{};
		std::unordered_map<std::string, std::vector<EventListener*>> m_Listeners{};
	};
}


