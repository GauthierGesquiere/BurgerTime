#pragma once
namespace dae
{
	class GameObject;
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Startup();
		virtual void Update(float deltaSec);
		virtual void Render() const;

		void SetOwner(GameObject* newOwner);
		GameObject* GetOwner() const;

	protected:
		GameObject* m_pOwner = nullptr;
	};
}

