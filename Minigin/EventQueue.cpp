#include "MiniginPCH.h"
#include "EventQueue.h"

#include "EventListener.h"

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
		m_Listeners[eventType] = std::vector{ pListener };
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
		const auto& pEvent = m_EventQue.front();
		m_EventQue.pop();

		for (EventListener* listener : m_Listeners[pEvent->Message])
		{
			listener->OnEvent(pEvent);
		}

		delete pEvent;
	}
}
