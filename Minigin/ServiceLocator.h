#pragma once
#include "Singleton.h"

namespace dae
{
	class AudioService;
	class ServiceLocator : public Singleton<ServiceLocator>
	{
	public:
		ServiceLocator(const ServiceLocator& other) = delete;
		ServiceLocator(ServiceLocator&& other) = delete;
		ServiceLocator& operator=(const ServiceLocator& other) = delete;
		ServiceLocator& operator=(ServiceLocator&& other) = delete;
		~ServiceLocator() override;

		AudioService* GetAudioService() const;
		void RegisterAudioService(AudioService* pAudioService);

	private:
		friend class Singleton<ServiceLocator>;
		ServiceLocator();

		AudioService* m_pAudioService;
		AudioService* m_pDefaultAudioService;
	};
}

