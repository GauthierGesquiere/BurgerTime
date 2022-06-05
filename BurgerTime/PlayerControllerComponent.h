#pragma once
#include <vector>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "ControllerComponent.h"
#include "EventListener.h"
#include "Subject.h"




class PlayerControllerComponent : public ControllerComponent, public dae::Subject, public dae::EventListener
{
public:
	PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int playerDims, glm::vec2 playerSize);
	//PlayerControllerComponent() = default;
	~PlayerControllerComponent() override;
	PlayerControllerComponent(const PlayerControllerComponent& other) = delete;
	PlayerControllerComponent(PlayerControllerComponent&& other) = delete;
	PlayerControllerComponent& operator=(const PlayerControllerComponent& other) = delete;
	PlayerControllerComponent& operator=(PlayerControllerComponent&& other) = delete;

	void UpdateMovement(MoveDirections dir);
	void ThrowSalt() const;
	bool OnEvent(const dae::Event* event) override;

private:
	void Startup() override;
	void Update(float deltaSec) override;

	void UpdateLeft();
	void UpdateRight();
	void UpdateDown();
	void UpdateUp();

	void UpdateReset();

	void AddInput();

	bool m_IsDead{};

};





