#include "Audio.h"

namespace spe
{
	Audio::Audio(const std::string &path, const AudioType type, const std::string& name) {
		Configure(path, type, name);
	}
	// BASICS

	void Audio::Configure(const std::string &path, const AudioType type, const std::string &name) {
		m_Path = path;
		m_AudioType = type;
		m_Name = name;
		if (type == AudioType::SOUND) {
			m_SoundSource = std::make_unique<sf::Sound>();
		}
		else {
			m_SoundSource = std::make_unique<sf::Music>();
		}
		isConfigured = true;
	}

	bool Audio::Load() {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return false;
		}
		if (m_AudioType == AudioType::SOUND) {
			m_SoundBuffer = sf::SoundBuffer();
			if (!m_SoundBuffer.loadFromFile(m_Path)) {
				std::cerr << "Error while loading sound file." << std::endl;
				return false;
			}
			static_cast<sf::Sound*>(m_SoundSource.get())->setBuffer(m_SoundBuffer);
		}
		else if (m_AudioType == AudioType::MUSIC) {
			if (!static_cast<sf::Music*>(m_SoundSource.get())->openFromFile(m_Path)) {
				std::cerr << "Error while loading music file." << std::endl;
				return false;
			}
		}

		return true;
	}


	void Audio::Play() {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		m_SoundSource->play();
	}

	void Audio::Stop() {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		m_SoundSource->stop();
	}

	void Audio::Pause() {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		m_SoundSource->pause();
	}

	sf::SoundSource::Status Audio::GetStatus() const {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return sf::SoundSource::Status::Stopped;
		}
		return m_SoundSource->getStatus();
	}

	// SOUND BUFFER AND SOUND, OR MUSIC GETTERS

	std::string Audio::GetPath() const {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return "";
		}
		return m_Path;
	}

	sf::SoundBuffer Audio::GetSoundBuffer() const {
		if (!isConfigured) {
			throw std::runtime_error("Audio is not configured yet.");
		}
		if (m_AudioType == AudioType::SOUND) {
			return m_SoundBuffer;
		}
		else {
			throw std::runtime_error("GetSoundBuffer is only available for sound.");
		}
	}

	sf::Music* Audio::GetMusicObject() const {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			throw std::runtime_error("Audio is not configured yet.");
		}
		else if (m_AudioType == AudioType::MUSIC) {
			return static_cast<sf::Music*>(m_SoundSource.get());
		}
		else {
			throw std::runtime_error("GetMusic is only available for music.");

		}
	}

	sf::Sound* Audio::GetSoundObject() const {
		if (!isConfigured) {
			throw std::runtime_error("Audio is not configured yet.");
		}
		if (m_AudioType == AudioType::SOUND) {
			return static_cast<sf::Sound*>(m_SoundSource.get());
		}
		else {
			throw std::runtime_error("GetSound is only available for sound.");
		}
	}


	// LOOPS
	void Audio::SetLoop(bool on) {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		if (m_AudioType == AudioType::SOUND) {
			static_cast<sf::Sound*>(m_SoundSource.get())->setLoop(on);
		}
		else if (m_AudioType == AudioType::MUSIC) {
			static_cast<sf::Music*>(m_SoundSource.get())->setLoop(on);
		}
		else {
			throw std::runtime_error("SetLoop is only available for sound and music.");
		}
	}

	bool Audio::GetLoop() const {
		if (!isConfigured) {
			throw std::runtime_error("Audio is not configured yet.");
		}
		if (m_AudioType == AudioType::SOUND) {
			return static_cast<sf::Sound*>(m_SoundSource.get())->getLoop();
		}
		if (m_AudioType == AudioType::MUSIC) {
			return static_cast<sf::Music*>(m_SoundSource.get())->getLoop();
		}
		throw std::runtime_error("GetLoop is only available for sound and music.");
	}

	void Audio::SetLoopPoints(float offset, float length) {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		if (m_AudioType == AudioType::MUSIC) {
			sf::Music::TimeSpan timeSpan(sf::seconds(offset), sf::seconds(length));
			static_cast<sf::Music*>(m_SoundSource.get())->setLoopPoints(timeSpan);
		}
		else {
			throw std::runtime_error("SetLoopPoints is only available for music.");
		}
	}

	// VOLUME
	void Audio::SetVolume(float vol) {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		if (m_AudioType == AudioType::SOUND) {
			static_cast<sf::Sound*>(m_SoundSource.get())->setVolume(vol);
		}
		else if (m_AudioType == AudioType::MUSIC) {
			static_cast<sf::Music*>(m_SoundSource.get())->setVolume(vol);
		}
		else {
			throw std::runtime_error("SetVolume is only available for sound and music.");
		}
	}

	float Audio::GetVolume() const {
		if (!isConfigured) {
			throw std::runtime_error("Audio is not configured yet.");
		}
		if (m_AudioType == AudioType::SOUND) {
			return static_cast<sf::Sound*>(m_SoundSource.get())->getVolume();
		}
		else if (m_AudioType == AudioType::MUSIC) {
			return static_cast<sf::Music*>(m_SoundSource.get())->getVolume();
		}
		throw std::runtime_error("GetVolume is only available for sound and music.");
	}

	// PITCH

	void Audio::SetPitch(float pitch) {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		if (m_AudioType == AudioType::SOUND) {
			static_cast<sf::Sound*>(m_SoundSource.get())->setPitch(pitch);
		}
		else if (m_AudioType == AudioType::MUSIC) {
			static_cast<sf::Music*>(m_SoundSource.get())->setPitch(pitch);
		}
	}

	float Audio::GetPitch() const {
		if (!isConfigured) {
			throw std::runtime_error("Audio is not configured yet.");
		}
		if (m_AudioType == AudioType::SOUND) {
			return static_cast<sf::Sound*>(m_SoundSource.get())->getPitch();
		}
		if (m_AudioType == AudioType::MUSIC) {
			return static_cast<sf::Music*>(m_SoundSource.get())->getPitch();
		}
		throw std::runtime_error("GetPitch is only available for sound and music.");
	}

	// POSITION

	void Audio::SetRelativeToListener(bool relative) {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		if (m_AudioType == AudioType::SOUND) {
			static_cast<sf::Sound*>(m_SoundSource.get())->setRelativeToListener(relative);
		}
		else {
			throw std::runtime_error("SetRelativeToListener is only available for sound.");
		}
	}

	void Audio::SetPosition(float x, float y) {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		if (m_AudioType == AudioType::SOUND) {
			static_cast<sf::Sound*>(m_SoundSource.get())->setPosition(x, y, 0.f);
		}
		else {
			throw std::runtime_error("SetPosition is only available for sound.");
		}
	}

	void Audio::SetMinDistance(float distance) {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		if (m_AudioType == AudioType::SOUND) {
			static_cast<sf::Sound*>(m_SoundSource.get())->setMinDistance(distance);
		}
		else {
			throw std::runtime_error("SetMinDistance is only available for sound.");
		}
	}

	void Audio::SetAttenuation(float attenuation) {
		if (!isConfigured) {
			std::cerr << "Audio is not configured yet." << std::endl;
			return;
		}
		if (m_AudioType == AudioType::SOUND) {
			static_cast<sf::Sound*>(m_SoundSource.get())->setAttenuation(attenuation);
		}
		else {
			throw std::runtime_error("SetAttenuation is only available for sound.");
		}
	}

	// DURATION

	int Audio::GetDuration() const {
		if (!isConfigured) {
			throw std::runtime_error("Audio is not configured yet.");
		}
		sf::Time duration;
		if (m_AudioType == AudioType::SOUND) {
			duration = static_cast<sf::Sound*>(m_SoundSource.get())->getBuffer()->getDuration();
		}
		else if (m_AudioType == AudioType::MUSIC) {
			duration = static_cast<sf::Music*>(m_SoundSource.get())->getDuration();
		}
		else {
			throw std::runtime_error("GetDuration is only available for sound and music.");
		}
		return duration.asMilliseconds();
	}

	// LISTENER

	std::string Audio::GetCsvString() const {
		if (!isConfigured) {
			throw std::runtime_error("Audio is not configured yet.");
		}
		return GetName() + ";" + GetPath() + ";" +
			(m_AudioType == AudioType::SOUND ? "Sound" : "Music") + ";" +
			(PlayOnStart ? "True" : "False") + ";" +
			std::to_string(GetVolume()) + ";" +
			std::to_string(GetPitch()) + ";" +
			(m_AudioType == AudioType::SOUND ? (m_SoundSource->isRelativeToListener() ? "True" : "False") : "False") + ";" +
			(m_AudioType == AudioType::SOUND ? std::to_string(m_SoundSource->getMinDistance()) : "-1") + ";" +
			(m_AudioType == AudioType::SOUND ? std::to_string(m_SoundSource->getAttenuation()) : "-1") + ";" +
			(GetLoop() ? "True" : "False") + ";" +
			(m_AudioType == AudioType::MUSIC ? (std::to_string(static_cast<sf::Music*>(m_SoundSource.get())->getLoopPoints().offset.asSeconds()) + "," +
				std::to_string(static_cast<sf::Music*>(m_SoundSource.get())->getLoopPoints().length.asSeconds())) : "-1,-1");
	}

	void Audio::SetGlobalVolume(float vol) {
		sf::Listener::setGlobalVolume(vol);
	}

	float Audio::GetGlobalVolume() {
		return sf::Listener::getGlobalVolume();
	}

	void Audio::SetListenerPosition(float x, float y) {
		//std::cout << "Listener position: " << x << ", " << y << std::endl;
		sf::Listener::setPosition(x, y, 0.f);
	}
}

// If you want to use the up vector, listener direction and 3d listner positioning,
// you have to access the sf::Listener class directly, 
// since this wrapper aims to be simple and easy to use and primarily for 2D games.


