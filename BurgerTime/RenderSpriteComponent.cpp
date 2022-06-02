#include "RenderSpriteComponent.h"

#include <iostream>
#include <SDL_render.h>

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

RenderSpriteComponent::RenderSpriteComponent(const std::string& path, unsigned int frameWidth, unsigned int frameHeight, unsigned int displayWidth, unsigned int displayHeight, LoopType loopType, bool mirror, float timeBetweenFrames)
	: m_IsInitialized{ true }
{
	SetTextureToDraw(path, frameWidth, frameHeight, timeBetweenFrames, displayWidth, displayHeight, loopType, mirror);
}

void RenderSpriteComponent::Startup()
{
}

void RenderSpriteComponent::Update(float deltaSec)
{
	if (!m_IsInitialized || m_FreezeOnFrame != -1)
	{
		return;
	}

	m_ElapsedSec += deltaSec;

	//Check what loop Type
	if (m_LoopType == LoopType::ForwardReverseLoop)
	{
		ForwardReverseLoop();
	}
	else if (m_LoopType == LoopType::Loop)
	{
		NormalLoop();
	}
	else
	{
		if (m_ElapsedSec > m_TimeBetweenFrames)
		{
			m_ElapsedSec = 0.0f;

			SetFrame(++m_CurrentFrame);
		}
	}
}

void RenderSpriteComponent::Render() const
{
	if (!m_IsInitialized)
	{
		return;
	}

	dae::Renderer::GetInstance().RenderTexture(*m_pTexture, m_SourceRect, m_pOwner->GetTransform().GetPosition().x, m_pOwner->GetTransform().GetPosition().y, static_cast<float>(m_DestinationWidth), static_cast<float>(m_DestinationHeight), m_MirrorSourceTexture);
}

void RenderSpriteComponent::SetFrame(unsigned frame)
{
	const unsigned int frameRow = frame / m_Columns;
	const unsigned int frameCol = frame % m_Columns;

	if ((frameRow + 1) > m_Rows || (frameCol + 1) > m_Columns)
		return; // This frame does not exist

	m_SourceRect.x = m_SourceRect.w * frameCol;
	m_SourceRect.y = m_SourceRect.h * frameRow;

	m_CurrentFrame = frame;
}

void RenderSpriteComponent::SetTextureToDraw(const std::string& path, unsigned frameWidth, unsigned frameHeight, float timeBetweenFrames, unsigned displayWidth, unsigned displayHeight, LoopType loopType, int freezeOnFrame, bool mirror)
{
	m_IsInitialized = true;

	m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(path);
	m_SourceRect = { 0, 0, static_cast<int>(frameWidth), static_cast<int>(frameHeight) };
	m_TimeBetweenFrames = timeBetweenFrames;
	m_DestinationWidth = displayWidth;
	m_DestinationHeight = displayHeight;
	m_CurrentFrame = 0;
	m_LoopType = loopType;
	m_FreezeOnFrame = freezeOnFrame;
	m_MirrorSourceTexture = mirror;

	int totalWidth;
	int totalHeight;

	SDL_QueryTexture(m_pTexture->GetSDLTexture(), nullptr, nullptr, &totalWidth, &totalHeight);

	m_Rows = totalHeight / frameHeight;
	m_Columns = totalWidth / frameWidth;

	if (m_DestinationWidth == 0)
	{
		m_DestinationWidth = frameWidth;
	}
	if (m_DestinationHeight == 0)
	{
		m_DestinationHeight = frameHeight;
	}

	if (m_FreezeOnFrame != -1)
	{
		m_CurrentFrame = m_FreezeOnFrame;
	}

	SetFrame(m_CurrentFrame);

}

bool RenderSpriteComponent::HasReachedLastFrame(unsigned frame) const
{
	return (m_Rows * m_Columns) - 1 <= frame ? true : false;
}

bool RenderSpriteComponent::HasReachedFirstFrame(unsigned frame)
{
	return 0 >= frame ? true : false;
}

void RenderSpriteComponent::ForwardReverseLoop()
{
	if (m_ElapsedSec > m_TimeBetweenFrames)
	{
		m_ElapsedSec = 0.0f;

		if (HasReachedLastFrame(m_CurrentFrame))
		{
			m_FrameModifier = -1;
		}
		if (HasReachedFirstFrame(m_CurrentFrame))
		{
			m_FrameModifier = 1;
		}

		m_CurrentFrame += m_FrameModifier;

		SetFrame(m_CurrentFrame);
	}
}

void RenderSpriteComponent::NormalLoop()
{
	if (m_ElapsedSec > m_TimeBetweenFrames)
	{
		m_ElapsedSec = 0.0f;

		if (HasReachedLastFrame(m_CurrentFrame))
		{
			m_CurrentFrame = -1;
		}

		SetFrame(++m_CurrentFrame);
	}
}
