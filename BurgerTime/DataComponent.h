#pragma once
#include <memory>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "Component.h"
#include "EventListener.h"

class DataComponent : public dae::Component, public dae::EventListener
{
public:
	DataComponent(glm::vec2 sourceToDestRatio);
	~DataComponent() override = default;
	DataComponent(const DataComponent& other) = delete;
	DataComponent(DataComponent&& other) = delete;
	DataComponent& operator=(const DataComponent& other) = delete;
	DataComponent& operator=(DataComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;

	bool OnEvent(const dae::Event* event) override;

protected:
	bool m_IsFirstUpdate{};


	std::string m_SourcePath{};

	glm::vec2 m_SourceToDestRatio{};


};

