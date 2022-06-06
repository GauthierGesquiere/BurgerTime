#pragma once
#include "Component.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "Transform.h"
#pragma warning(pop)

enum class BurgerPiece
{
	TopBread,
	Tomato,
	Salad,
	Meat,
	Cheese,
	BottomBread
};

class BurgerPieceComponent : public dae::Component
{
public:
	BurgerPieceComponent(glm::vec2 pos, unsigned int amountOfDrops, BurgerPiece piece, glm::vec2 sourceToDestRatio, std::vector<std::vector<glm::vec2>>* pLevelIndices);
	~BurgerPieceComponent() override = default;
	BurgerPieceComponent(const BurgerPieceComponent& other) = delete;
	BurgerPieceComponent(BurgerPieceComponent&& other) = delete;
	BurgerPieceComponent& operator=(const BurgerPieceComponent& other) = delete;
	BurgerPieceComponent& operator=(BurgerPieceComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;

	void SetPlayerTransform(dae::Transform* pPlayerTransform);
	void UpdateGravity(float deltaSec);
	bool CheckPieceAbove(BurgerPieceComponent* other);

	bool m_IsInitialized{};


private:
	void CheckIfPlayerCoveredPiece(float deltaSec);
	const utils::Rectf CalculateBox(glm::vec2 posToCalc = { -1, -1 }) const;
	bool HitFloor();

	unsigned int m_AmountOfDrops{};
	glm::vec2 m_Position{};
	BurgerPiece m_Piece{};

	std::string m_SourcePathg{};

	glm::vec2 m_SourceDims{};
	unsigned int m_FreezeFrame{};
	glm::vec2 m_SourceToDestRatio{};

	std::shared_ptr<dae::GameObject> m_PieceObj{};

	bool m_isFirstUpdate{};
	dae::Transform* m_pPlayerTransform{};

	bool m_HasRightSide{};
	bool m_HasLeftSide{};
	bool m_HasMid{};

	bool m_PieceFellOn{};
	bool m_NeedsToStack{};

	glm::vec2 m_Gravity{};
	glm::vec2 m_Velocity{};

	std::vector<std::vector<glm::vec2>>* m_pLevelIndices{};

	float ElapsedSec{};

	bool BroadcastOnce{};
};

