#pragma once
#include "Singleton.h"
#include <queue>
#include <unordered_map>
#include "Event.h"
//#include <vector>

namespace dae
{
	class EventListener;
	class EventQueue final : public Singleton<EventQueue>
	{
	public:
		EventQueue() = default;
		~EventQueue() override;
		EventQueue(const EventQueue& other) = delete;
		EventQueue(EventQueue&& other) = delete;
		EventQueue& operator=(const EventQueue& other) = delete;
		EventQueue& operator=(EventQueue&& other) = delete;

		void Broadcast(Event* event);

		void Subscribe(const std::string& eventType, EventListener* pListener);

		//For cleanup
		void Unsubscribe(const std::string& eventType, EventListener* pListener);

		void HandleEventQueue();

	private:
		std::queue<Event*> m_EventQue{};
		std::unordered_map<std::string, std::vector<EventListener*>> m_Listeners{};
	};
}


