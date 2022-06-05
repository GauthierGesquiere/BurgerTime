#pragma once
#include "Component.h"

#include <vector>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>

#include "utils.h"
#pragma warning(pop)

enum class MoveDirections
{
	Left,
	Right,
	Up,
	Down
};

class ControllerComponent : public dae::Component
{
public:
	ControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int Dims, glm::vec2 Size);
	//PlayerControllerComponent() = default;
	~ControllerComponent() override = default;
	ControllerComponent(const ControllerComponent& other) = delete;
	ControllerComponent(ControllerComponent&& other) = delete;
	ControllerComponent& operator=(const ControllerComponent& other) = delete;
	ControllerComponent& operator=(ControllerComponent&& other) = delete;

protected:
	void Startup() override;
	void Update(float deltaSec) override;

	bool HitFloor();
	bool HitEdge();
	bool HitLadder();

	void UpdateGravity(float deltaTime);
	void TranslateSprite(float deltaTime) const;

	const utils::Rectf CalculateBox(glm::vec2 posToCalc = {-1, -1}) const;

	std::vector<std::vector<glm::vec2>>* m_pLevelIndices{};

	glm::vec2 m_Gravity{};
	glm::vec2 m_Velocity{};

	glm::vec2 m_Size{};
	unsigned int m_Dims{};

	float m_MovementSpeed{};

	bool m_HitLadder{};

	enum class LadderState
	{
		UpDown,
		Down,
		Up,
		NoLadder
	};

	LadderState m_LadderState{};
};

