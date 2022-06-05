#pragma once
#include "Observer.h"

namespace dae
{

	class Subject
	{
	public:
		void AddObserver(Observer* pObserver)
		{
			m_pObservers.push_back(pObserver);
		}

		void RemoveObserver(Observer* pObserver)
		{
			const auto Idx = std::remove(m_pObservers.begin(), m_pObservers.end(), pObserver);
		}

		void Notify(const GameObject& actor, Event* event) const
		{
			for (const auto& observer : m_pObservers)
			{
				observer->OnNotify(actor, event);
			}
		}

	private:
		std::vector<Observer*> m_pObservers{};
	};
}

