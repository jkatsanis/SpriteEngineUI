#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>

namespace spe
{
	class Audio {
	public:
		enum class AudioType { SOUND, MUSIC };
		bool PlayOnStart = false;

		Audio() = default;
		Audio(const std::string &path, AudioType type, const std::string &name); 	// constructor
		void Configure(const std::string &path, AudioType type,const std::string &name);	// configures the audio
		bool Load();														// loads the audio
		void Play();														// starts the audio
		void Stop();														// stops the audio
		void Pause();														// pauses the audio

		// SOUND BUFFER AND SOUND, OR MUSIC GETTERS
		// (for if you want to use stuff the wrapper doesnt provide)
		sf::SoundSource::Status GetStatus() const;							// gets the status of the audio
		sf::SoundBuffer GetSoundBuffer() const;								// gets the soundbuffer of the sound, only works for sound
		sf::Music* GetMusicObject() const;									// gets the music object
		sf::Sound* GetSoundObject() const;									// gets the sound object

		// SETTERS
		void SetLoop(bool on);												// turns looping of the audio on or off (depending on the boolean)
		void SetLoopPoints(float offset, float length);							// sets the loop points of the audio (in seconds)
		void SetVolume(float vol);											// sets the volume of the audio (0-100)
		void SetPitch(float pitch);											// sets the pitch of the audio (1 = normal pitch)
		void SetPosition(float x, float y);									// sets the position of the audio
		void SetRelativeToListener(bool relative);							// sets whether the audio is relative to the listener (mono audio required for fixed position)
		//			- true => sound is always at listeners position
		//			- false => sound is fixed and changes volume depending on distance
		void SetMinDistance(float distance);								// sets the minimum distance of the audio
		void SetAttenuation(float attenuation);								// sets the attenuation of the audio
		void SetName(const std::string &name) { m_Name = name; }			// sets the name of the audio

		// GETTERS
		bool GetLoop() const;												// gets whether the audio is in looping mode
		float GetVolume() const;											// gets the volume of the audio (0-100)
		float GetPitch() const;												// gets the pitch of the audio (1 = normal pitch)
		int GetDuration() const;											// gets the duration of the audio in ms. Returns -1 if something went wrong.
		std::string GetPath() const;										// gets the path of the audio
		std::string GetName() const { return m_Name;}					// gets the name of the audio
		std::string GetCsvString() const;									// gets the csv string: NAME;PATH;TYPE;PLAYONSTART;VOLUME;PITCH;RELATIVE;MINDISTANCE;ATTENUATION;LOOP;LOOPPOINTS
		AudioType GetType() const { return m_AudioType; }							// gets the type of the audio (sound or music)
		// STATIC FUNCTIONS
		static void SetGlobalVolume(float vol);								// sets the global volume of all audio
		static float GetGlobalVolume();										// gets the global volume of all audio
		static void SetListenerPosition(float x, float y);					// sets the position of the listener; x = left/right, y = down/up

	private:
		std::string m_Path;													// the path to the audio file
		AudioType m_AudioType;												// the type of the audio (sound or music)
		std::unique_ptr<sf::SoundSource> m_SoundSource;						// polymorphic sound source
		sf::SoundBuffer m_SoundBuffer;										// sound buffer for sounds
		bool isConfigured = false;
		std::string m_Name;													// name of the audio
	};
}
