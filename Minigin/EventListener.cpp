#include "MiniginPCH.h"
#include "EventListener.h"
#include "EventQueue.h"


dae::EventListener::~EventListener()
{
	dae::EventQueue::GetInstance().Unsubscribe(this);
}

