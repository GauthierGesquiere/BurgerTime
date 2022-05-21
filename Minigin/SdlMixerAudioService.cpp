#include "MiniginPCH.h"
#include "SdlMixerAudioService.h"

dae::SdlMixerAudioService::SdlMixerAudioService()
{
	SDL_Init(SDL_INIT_AUDIO);
	const int flags = MIX_INIT_MP3;
	Mix_Init(flags);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
}

dae::SdlMixerAudioService::~SdlMixerAudioService()
{
	for (const auto audioPair : m_AudioMap)
	{
		Mix_FreeMusic(audioPair.second);
	}
	Mix_Quit();
}

void dae::SdlMixerAudioService::PlayMusic(const std::string& filename, int volumePercentage)
{
	std::lock_guard lock{ m_Mutex };
	m_SoundRequestQueue.push(AudioRequest{ AudioType::music, filename, volumePercentage });
}

void dae::SdlMixerAudioService::PlaySound(const std::string& filename, int volumePercentage)
{
	std::lock_guard lock{ m_Mutex };
	m_SoundRequestQueue.push(AudioRequest{ AudioType::sound, filename, volumePercentage });
}

void dae::SdlMixerAudioService::ProcessSoundRequests()
{
	while (!m_SoundRequestQueue.empty())
	{
		std::lock_guard lock{ m_Mutex };

		AudioRequest request = m_SoundRequestQueue.front();
		Mix_VolumeMusic(request.volumePercentage);
		m_SoundRequestQueue.pop();

		auto findResult = m_AudioMap.find(request.filename);
		if (findResult == m_AudioMap.end())
		{
			AddNewAudioToSavedAudio(request);
		}
		if (request.audioType == AudioType::music)
		{
			if (Mix_PlayMusic(m_AudioMap[request.filename], static_cast<uint8_t>(request.audioType)) == -1) {
				printf("Mix_PlayMusic: %s\n", Mix_GetError());
				// well, there's no music, but most games don't break without music...
			}
		}
		else
		{
			Mix_PlayMusic(m_AudioMap[request.filename], static_cast<uint8_t>(request.audioType));
		}
	}
}

void dae::SdlMixerAudioService::AddNewAudioToSavedAudio(const AudioRequest& newAudioRequest)
{
	Mix_Music* pAudio = Mix_LoadMUS(newAudioRequest.filename.c_str());
	if (!pAudio)
	{
		std::wcout << L"Could not find Audio File";
	}
	m_AudioMap.insert({ newAudioRequest.filename, pAudio });
}
