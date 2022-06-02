#pragma once
#include <memory>
#include <SDL_rect.h>
#include <string>

#include "Component.h"
#include "Texture2D.h"

enum class LoopType
{
	Loop,
	ForwardReverseLoop,
	NoLoop
};

class RenderSpriteComponent : public dae::Component
{
public:
	RenderSpriteComponent() = default;
	RenderSpriteComponent(const std::string& path, unsigned int frameWidth, unsigned int frameHeight, unsigned int displayWidth = 0, unsigned int displayHeight = 0, LoopType loopType = LoopType::Loop, bool mirror = false, float timeBetweenFrames = 0.0f);
	~RenderSpriteComponent() override = default;
	RenderSpriteComponent(const RenderSpriteComponent& other) = delete;
	RenderSpriteComponent(RenderSpriteComponent&& other) = delete;
	RenderSpriteComponent& operator=(const RenderSpriteComponent& other) = delete;
	RenderSpriteComponent& operator=(RenderSpriteComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;
	void Render() const override;

	void SetFrame(unsigned int frame);
	void SetTextureToDraw(const std::string& path, unsigned int frameWidth, unsigned int frameHeight,
	                      float timeBetweenFrames, unsigned int displayWidth = 0, unsigned int displayHeight = 0,
	                      LoopType loopType = LoopType::Loop, int freezeOnFrame = -1, bool mirror = false);
	float GetTotalAmountOfTime() const { return static_cast<float>(m_Columns) * static_cast<float>(m_Rows) * m_TimeBetweenFrames; }

private:
	std::shared_ptr<dae::Texture2D> m_pTexture{};

	SDL_Rect m_SourceRect{};

	unsigned int m_Rows{};
	unsigned int m_Columns{};

	unsigned int m_DestinationWidth{};
	unsigned int m_DestinationHeight{};

	int m_CurrentFrame;

	float m_ElapsedSec{ 0.0f };

	LoopType m_LoopType{};
	int m_FrameModifier{ 1 };

	bool m_MirrorSourceTexture{};

	float m_TimeBetweenFrames{};

	bool m_IsInitialized{ false };

	int m_FreezeOnFrame{};

	bool HasReachedLastFrame(unsigned int frame) const;
	static bool HasReachedFirstFrame(unsigned int frame);

	void ForwardReverseLoop();
	void NormalLoop();
};
