#include "MiniginPCH.h"
#include "EventQueue.h"

dae::EventQueue::~EventQueue()
{
}

void dae::EventQueue::Broadcast(Event* event)
{
	m_EventQue.push(event);
}

void dae::EventQueue::Subscribe(const std::string& eventType, EventListener* pListener)
{
	//Looks if listeners have this event type
	if (m_Listeners.find(eventType) != m_Listeners.end())
	{
		//check if this event has this listener
		const auto& itr = std::find(
			m_Listeners[eventType].begin(),
			m_Listeners[eventType].end(),
			pListener);

		//if not add it
		if (itr == m_Listeners[eventType].end())
		{
			m_Listeners[eventType].push_back(pListener);
		}
	}
	else
	{
		//if not add it
		m_Listeners[eventType] = std::vector<EventListener*>{ pListener };
	}
}

void dae::EventQueue::Unsubscribe(EventListener* pListener)
{
	//unsibscribe all events for this listener
	for (auto& pair : m_Listeners)
	{
		Unsubscribe(pair.first, pListener);
	}
}

void dae::EventQueue::Unsubscribe(const std::string& eventType, EventListener* pListener)
{
	//Look if event has listeners
	if (m_Listeners.find(eventType) != m_Listeners.end())
	{
		//find listeners
		const auto listenerItr = std::find(
			m_Listeners[eventType].begin(),
			m_Listeners[eventType].end(),
			pListener);

		if (listenerItr != m_Listeners[eventType].end())
		{
			//remove them
			m_Listeners[eventType].erase(listenerItr);
		}
	}
}

void dae::EventQueue::HandleEventQueue()
{
	while (!m_EventQue.empty())
	{
		auto& pEvent = m_EventQue.front();

		//for (EventListener* listener : m_Listeners[pEvent->Message])
		//{
		//	listener->OnEvent(*pEvent);
		//}

		delete pEvent;
		m_EventQue.pop();
	}
}
