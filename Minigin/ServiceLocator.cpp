#include "MiniginPCH.h"
#include "ServiceLocator.h"

#include "AudioService.h"

dae::ServiceLocator::ServiceLocator()
	: m_pDefaultAudioService(new NullAudioService())
	, m_pAudioService(nullptr)
{
}

dae::ServiceLocator::~ServiceLocator()
{
	if (m_pAudioService)
	{
		delete m_pAudioService;
		m_pAudioService = nullptr;
	}
	delete m_pDefaultAudioService;
	m_pDefaultAudioService = nullptr;
}

void dae::ServiceLocator::RegisterAudioService(AudioService* pAudioService)
{
	if (!pAudioService)
	{
		m_pAudioService = m_pDefaultAudioService;
	}
	else
	{
		m_pAudioService = pAudioService;
	}
}

dae::AudioService* dae::ServiceLocator::GetAudioService() const
{
	if (!m_pAudioService)
	{
		return m_pDefaultAudioService;
	}
	return m_pAudioService;
}