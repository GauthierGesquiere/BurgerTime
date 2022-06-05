#include "ControllerComponent.h"

#include "Event.h"
#include "GameObject.h"

ControllerComponent::ControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned playerDims, glm::vec2 playerSize)
	: m_pLevelIndices{ pLevelIndices }
	, m_Gravity{ 0, 9 }
	, m_Velocity{ 0, 0 }
	, m_Size{ playerSize }
	, m_Dims{ playerDims }
	, m_MovementSpeed{ 50 }
{
}

void ControllerComponent::Startup()
{
	Component::Startup();
}

void ControllerComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);
}


bool ControllerComponent::HitFloor()
{
	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	const glm::vec2 startP{ box.left + box.width / 2, box.bottom - 2 };
	const glm::vec2 endP{ box.left + box.width / 2, box.bottom + 2 };

	/*
 *		+--------+
 *		|        |
 *		|        |
 *		|        |
 *		|    |   |
 *		|    |   |
 *		+----+---+
 *		     |
 */

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			return true;
		}
	}
	return false;
}

bool ControllerComponent::HitEdge()
{
	if (m_Velocity.x == 0.0f)
	{
		return false;
	}

	bool hitEdge = false;

	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	glm::vec2 startP{ box.left + box.width / 2, box.bottom - 5 };
	glm::vec2 endP;

	if (m_Velocity.x > 0)
	{
		/*		Right
		 *		+--------+
		 *		|        |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		|     ---+-
		 *		+--------+
		 */

		 //IsWalkingRight
		endP = glm::vec2{ box.left + box.width + 1, box.bottom - 5 };
	}
	else if (m_Velocity.x < 0)
	{
		/*		Left
		 *		+--------+
		 *		|        |
		 *		|        |
		 *		|        |
		 *		|        |
		 *	   -+---     |
		 *		+--------+
		 */

		 //IsWalkingLeft
		endP = glm::vec2{ box.left - 1, box.bottom - 5 };
	}

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			hitEdge = true;
		}
	}

	//HitEdge but still need to do other check
	if (hitEdge)
	{
		hitEdge = false;
		startP = { box.left + box.width / 2, box.bottom - box.height + 10 };
		if (m_Velocity.x > 0)
		{
			/*		Right
			 *		+--------+
			 *		|     ---+-
			 *		|        |
			 *		|        |
			 *		|        |
			 *		|        |
			 *		+--------+
			 */

			 //IsWalkingRight
			endP = { box.left + box.width + 1, box.bottom - box.height + 10 };
		}
		else
		{
			/*		Left
			 *		+--------+
			 *	   -+---     |
			 *		|        |
			 *		|        |
			 *		|        |
			 *		|        |
			 *		+--------+
			 */

			 //IsWalkingLeft
			endP = { box.left - 1, box.bottom - box.height + 10 };
		}

		for (const auto& levelIndices : *m_pLevelIndices)
		{
			if (Raycast(levelIndices, startP, endP, hitInfo))
			{
				hitEdge = true;
			}
		}

		if (hitEdge)
		{
			startP = { box.left + box.width / 2, box.bottom - box.height / 2 };
			if (m_Velocity.x > 0)
			{
				/*		Right
				 *		+--------+
				 *		|        |
				 *		|        |
				 *		|     ---+-
				 *		|        |
				 *		|        |
				 *		+--------+
				 */

				 //IsWalkingRight
				endP = { box.left + box.width + 1, box.bottom - box.height / 2 };
			}
			else
			{
				/*		Left
				 *		+--------+
				 *		|        |
				 *		|        |
				 *	   -+---     |
				 *		|        |
				 *		|        |
				 *		+--------+
				 */

				 //IsWalkingLeft
				endP = { box.left - 1, box.bottom - box.height / 2 };
			}

			for (const auto& levelIndices : *m_pLevelIndices)
			{
				if (Raycast(levelIndices, startP, endP, hitInfo))
				{
					hitEdge = false;
				}
			}

			if (hitEdge)
			{
				m_Velocity.x = 0.0f;
			}
		}
	}

	return hitEdge;
}

bool ControllerComponent::HitLadder()
{
	bool hitLadder = false;
	m_HitLadder = false;

	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	glm::vec2 startP{ box.left + box.width / 2 - 5, box.bottom - 3 };
	glm::vec2 endP{ box.left + box.width / 2 + 5, box.bottom - 3 };

	/*
	 *		+--------+
	 *		|        |
	 *		|        |
	 *		|        |
	 *		|        |
	 *		| ------ |
	 *		+--------+
	 */


	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			hitLadder = true;
		}
	}

	//HitEdge but still need to do other check
	if (hitLadder)
	{
		startP = { box.left + box.width / 2 - 5, box.bottom - box.height + 5 };
		endP = { box.left + box.width / 2 + 5, box.bottom - box.height + 5 };

		/*
		 *		+--------+
		 *		| ------ |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		+--------+
		 */

		for (const auto& levelIndices : *m_pLevelIndices)
		{
			if (Raycast(levelIndices, startP, endP, hitInfo))
			{
				hitLadder = false;
			}
		}

		if (hitLadder)
		{
			//Can go only Down
			//std::cout << "Can Go Only Down" << std::endl;
			m_HitLadder = true;
			if (HitFloor())
			{
				m_LadderState = LadderState::Down;
			}
		}
		else
		{
			//Can go Up and Down
			//std::cout << "Can Go Up, Down" << std::endl;
			m_HitLadder = true;
			m_LadderState = LadderState::UpDown;
		}
	}
	else
	{
		startP = { box.left + box.width / 2 - 5, box.bottom - box.height + 5 };
		endP = { box.left + box.width / 2 + 5, box.bottom - box.height + 5 };

		/*
		 *		+--------+
		 *		| ------ |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		|        |
		 *		+--------+
		 */

		for (const auto& levelIndices : *m_pLevelIndices)
		{
			if (Raycast(levelIndices, startP, endP, hitInfo))
			{
				hitLadder = true;
			}
		}

		if (hitLadder)
		{
			//Can go only Up
			//std::cout << "Can Go Only Up" << std::endl;
			m_HitLadder = true;
			if (HitFloor())
			{
				m_LadderState = LadderState::Up;
			}
		}
		else
		{
			//m_LadderState = LadderState::NoLadder;
			//m_HitLadder = false;
		}
	}

	return hitLadder;
}


void ControllerComponent::UpdateGravity(float deltaTime)
{
	m_Velocity += m_Gravity * deltaTime;
}

void ControllerComponent::TranslateSprite(float deltaTime) const
{
	m_pOwner->GetTransform().MoveWith(glm::vec3{ m_Velocity.x * deltaTime, m_Velocity.y * deltaTime, 0 });
}

const utils::Rectf ControllerComponent::CalculateBox(glm::vec2 posToCalc) const
{
	if (posToCalc == glm::vec2{-1, -1})
	{
		utils::Rectf box;
		const glm::vec2 topLeft = m_pOwner->GetTransform().GetPosition();
		box.left = topLeft.x;
		box.bottom = topLeft.y + m_Dims * m_Size.y;
		box.width = m_Dims * m_Size.x;
		box.height = m_Dims * m_Size.y;
		return box;
	}

	utils::Rectf box;
	const glm::vec2 topLeft = posToCalc;
	box.left = topLeft.x;
	box.bottom = topLeft.y + m_Dims * m_Size.y;
	box.width = m_Dims * m_Size.x;
	box.height = m_Dims * m_Size.y;
	return box;
}
