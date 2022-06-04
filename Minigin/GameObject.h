#pragma once
#include "Transform.h"
#include <vector>

namespace dae
{
	class Component;
	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float deltaSec);
		void Render() const;

		Transform& GetTransform() { return m_Transform; }

		void MarkForDelete();
		bool GetIsMarkedForDelete() const;


		//Components
		template<typename T>
		//std::enable_if_t<std::is_base_of_v<class Component, T>, T*>
		void AddComponent(T* pComp)
		{
			const auto findItrResult = std::find_if(m_Components.begin(), m_Components.end(),
				[pComp](Component* pComponent) { return pComponent == pComp; });

			if (findItrResult == m_Components.end())
			{
				m_Components.push_back(pComp);
				dynamic_cast<Component*>(pComp)->SetOwner(this);
			}
		}

		template<typename T>
		void RemoveComponent(T* pComp, bool deleteObject)
		{
			auto it = find(m_Components.begin(), m_Components.end(), pComp);

			m_Components.erase(it);
			dynamic_cast<Component*>(pComp)->SetOwner(nullptr);

			if (deleteObject)
			{
				delete pComp;
				pComp = nullptr;
			}
		}

		template <typename T>
		T* GetComponentOfType() const
		{
			for (Component* component : m_Components)
			{
				if (auto cast = dynamic_cast<T*>(component))
				{
					return cast;
				}
			}
			return nullptr;
		}

	private:
		Transform m_Transform;
		std::vector<Component*> m_Components{};
		bool m_IsMarkedForDeletion = false;
		bool m_IsFirstUpdate{ true };
	};
}
