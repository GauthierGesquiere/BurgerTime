#pragma once
namespace dae
{
	class AudioService
	{
	public:
		AudioService() = default;
		AudioService(const AudioService& other) = delete;
		AudioService(AudioService&& other) = delete;
		AudioService& operator=(const AudioService& other) = delete;
		AudioService& operator=(AudioService&& other) = delete;
		virtual ~AudioService() = default;

		virtual void PlayMusic(const std::string& filename, int volumePercentage) = 0;
		virtual void PlaySound(const std::string& filename, int volumePercentage) = 0;

		virtual void ProcessSoundRequests() = 0;
	};

	class NullAudioService final : public AudioService
	{
	public:
		NullAudioService() = default;
		NullAudioService(const NullAudioService& other) = delete;
		NullAudioService(NullAudioService&& other) = delete;
		NullAudioService& operator=(const NullAudioService& other) = delete;
		NullAudioService& operator=(NullAudioService&& other) = delete;
		virtual ~NullAudioService() = default;

		void PlayMusic(const std::string& /*filename*/, int /*volumePercentage*/) override {}
		void PlaySound(const std::string& /*filename*/, int /*volumePercentage*/) override {}
		void ProcessSoundRequests() override {}

	};
}

