#pragma once
#include "Component.h"
#include <vector>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>

#include "Subject.h"
#include "utils.h"
#pragma warning(pop)

enum class MoveDirections
{
	Left,
	Right,
	Up,
	Down
};

class PlayerControllerComponent : public dae::Component, public dae::Subject
{
public:
	PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int playerDims, glm::vec2 playerSize);
	//PlayerControllerComponent() = default;
	~PlayerControllerComponent() override = default;
	PlayerControllerComponent(const PlayerControllerComponent& other) = delete;
	PlayerControllerComponent(PlayerControllerComponent&& other) = delete;
	PlayerControllerComponent& operator=(const PlayerControllerComponent& other) = delete;
	PlayerControllerComponent& operator=(PlayerControllerComponent&& other) = delete;

	void UpdateMovement(MoveDirections dir);
	void ThrowSalt() const;

protected:
	void Startup() override;
	void Update(float deltaSec) override;


private:
	std::vector<std::vector<glm::vec2>>* m_pLevelIndices{};

	glm::vec2 m_Gravity{};
	glm::vec2 m_Velocity{};

	glm::vec2 m_PlayerSize{};
	unsigned int m_PlayerDims{};

	float m_MovementSpeed{};

	bool m_IsOnLadder{};
	bool m_HitLadder{};

	void UpdateGravity(float deltaTime);
	void TranslateSprite(float deltaTime) const;

	void UpdateLeft();
	void UpdateRight();
	void UpdateDown();
	void UpdateUp();

	void UpdateReset();

	bool HitFloor();
	bool HitEdge();
	bool HitLadder();

	const utils::Rectf CalculateBox() const ;
	void AddInput();

	enum class LadderState
	{
		UpDown,
		Down,
		Up,
		NoLadder
	};

	LadderState m_LadderState{};
};





