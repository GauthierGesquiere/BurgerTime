#include "DataComponent.h"

#include "RenderSpriteComponent.h"


DataComponent::DataComponent(glm::vec2 sourceToDestRatio)
	: m_IsFirstUpdate{ true }
	, m_SourcePath{"BurgerTime/Data/"}
	, m_SourceToDestRatio{ sourceToDestRatio }
{
}

void DataComponent::Startup()
{
	

	
}

void DataComponent::Update(float /*deltaSec*/)
{

}

bool DataComponent::OnEvent(const dae::Event* /*event*/)
{
	return false;
}
