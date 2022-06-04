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


class PlayerControllerComponent : public dae::Component
{
public:
	PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int playerDims, glm::vec2 playerSize);
	//PlayerControllerComponent() = default;
	~PlayerControllerComponent() override = default;
	PlayerControllerComponent(const PlayerControllerComponent& other) = delete;
	PlayerControllerComponent(PlayerControllerComponent&& other) = delete;
	PlayerControllerComponent& operator=(const PlayerControllerComponent& other) = delete;
	PlayerControllerComponent& operator=(PlayerControllerComponent&& other) = delete;

	void UpdateWalking(MoveDirections dir);

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

	void UpdateGravity(float deltaTime);
	void TranslateSprite(float deltaTime) const;

	bool HitFloor();

	const utils::Rectf CalculateBox() const ;
	void AddInput();
};





