#pragma once

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>

#include "utils.h"
#pragma warning(pop)

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void MoveWith(const glm::vec3 pos);
		void SetRect(utils::Rectf rect) { m_Rect = rect; }
		const utils::Rectf& GetRect() const { return m_Rect; }
	private:
		glm::vec3 m_Position;
		utils::Rectf m_Rect{};
	};
}
