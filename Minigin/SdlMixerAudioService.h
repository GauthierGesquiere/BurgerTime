#pragma once
#include <map>
#include <mutex>
#include <queue>
#include "../3rdParty/SDL2_mixer/include/SDL_mixer.h"

#include "AudioService.h"


namespace dae
{
	
	class SdlMixerAudioService final : public AudioService
	{
	public:
		SdlMixerAudioService();
		SdlMixerAudioService(const SdlMixerAudioService& other) = delete;
		SdlMixerAudioService(SdlMixerAudioService&& other) = delete;
		SdlMixerAudioService operator=(const SdlMixerAudioService& other) = delete;
		SdlMixerAudioService operator=(SdlMixerAudioService&& other) = delete;
		~SdlMixerAudioService();

		// Possibly misleading name: Adds a request to play this music/sound to the queue (Need to conform to Interface)
		void PlayMusic(const std::string& filename, int volumePercentage) override;
		void PlaySound(const std::string& filename, int volumePercentage) override;

		void ProcessSoundRequests() override;

	private:
		enum class AudioType
		{
			sound = 0,
			music = 5
		};
		struct AudioRequest
		{
			const AudioType audioType;
			const std::string filename;
			int volumePercentage;
		};

		std::mutex m_Mutex;
		std::queue<AudioRequest> m_SoundRequestQueue;
		std::map<std::string, Mix_Music*> m_AudioMap;

		void AddNewAudioToSavedAudio(const AudioRequest& newAudioRequest);
	};
}

