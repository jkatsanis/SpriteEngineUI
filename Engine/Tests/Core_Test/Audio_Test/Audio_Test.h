#pragma once
#include "gtest/gtest.h"
#include "Audio/Audio.h"
#include "UtilityFunctions.h"

// Skip all tests in the suite if it's the linux gh action
class AudioTestEnvironment final : public ::testing::Environment {
public:
    void SetUp() override {

            if (std::getenv("NO_AUDIO_DEVICE") != nullptr) {
                GTEST_SKIP() << "Skipping all AudioTests: no audio device on Linux";
            }
    }
};

// Register environment (runs before any tests)
static ::testing::Environment* const audio_env =
    ::testing::AddGlobalTestEnvironment(new AudioTestEnvironment);

class AudioTest : public ::testing::Test {
protected:
    std::string audioPath = "Tests/Core_Test/TestResources/audio.wav";
    spe::Audio sound;
    spe::Audio music;

    void SetUp() override {
        // Initialize fresh objects for every test
        sound = spe::Audio(audioPath, spe::Audio::AudioType::SOUND, "Sound Test");
        music = spe::Audio(audioPath, spe::Audio::AudioType::MUSIC, "Music Test");

        // Load them so they are ready to use
        if (!sound.Load()) {
            FAIL() << "Failed to load sound resource: " << audioPath;
        }
        if (!music.Load()) {
            FAIL() << "Failed to load music resource: " << audioPath;
        }
    }
};

TEST_F(AudioTest, Load) {
    EXPECT_EQ(sound.GetPath(), audioPath);
    EXPECT_EQ(music.GetPath(), audioPath);
}

TEST_F(AudioTest, GlobalVolume) {
    float vol = 32.0f;
    spe::Audio::SetGlobalVolume(vol);
    EXPECT_EQ(spe::Audio::GetGlobalVolume(), vol);
}

TEST_F(AudioTest, Volume) {
    if (std::getenv("NO_AUDIO_DEVICE")) {
    	GTEST_SKIP() << "Audio device not available, skipping test.";
    }
    float vol = 33.0f;
    sound.SetVolume(vol);
    music.SetVolume(vol);
    EXPECT_EQ(sound.GetVolume(), vol);
    EXPECT_EQ(music.GetVolume(), vol);
}

TEST_F(AudioTest, Loop) {
    sound.SetLoop(true);
    music.SetLoop(true);
    EXPECT_EQ(sound.GetLoop(), true);
    EXPECT_EQ(music.GetLoop(), true);
}

TEST_F(AudioTest, LoopPoints) {
    EXPECT_THROW(sound.SetLoopPoints(2, 3), std::runtime_error);
    EXPECT_NO_THROW(music.SetLoopPoints(2, 3));
}

TEST_F(AudioTest, PlayPauseStop) {
    if (std::getenv("NO_AUDIO_DEVICE")) {
    	GTEST_SKIP() << "Audio device not available, skipping test.";
    }

    sound.Play();
    music.Play();
    EXPECT_EQ(sound.GetStatus(), sf::SoundSource::Playing);
    EXPECT_EQ(music.GetStatus(), sf::SoundSource::Playing);

    sound.Pause();
    music.Pause();
    EXPECT_EQ(sound.GetStatus(), sf::SoundSource::Paused);
    EXPECT_EQ(music.GetStatus(), sf::SoundSource::Paused);

    sound.Stop();
    music.Stop();
    EXPECT_EQ(sound.GetStatus(), sf::SoundSource::Stopped);
    EXPECT_EQ(music.GetStatus(), sf::SoundSource::Stopped);
}

TEST_F(AudioTest, Pitch) {
    if (std::getenv("NO_AUDIO_DEVICE")) {
    	GTEST_SKIP() << "Audio device not available, skipping test.";
    }
    float pitch = 1.5f;
    sound.SetPitch(pitch);
    music.SetPitch(pitch);
    EXPECT_EQ(sound.GetPitch(), pitch);
    EXPECT_EQ(music.GetPitch(), pitch);
}

TEST_F(AudioTest, ListenerPosition) {
    float x = 10.0f, y = 20.0f;
    spe::Audio::SetListenerPosition(x, y);

    EXPECT_NO_THROW(sound.SetRelativeToListener(true));
	EXPECT_THROW(music.SetRelativeToListener(true), std::runtime_error);
}

TEST_F(AudioTest, Position) {
    float x = 5.0f, y = 10.0f;
    EXPECT_NO_THROW(sound.SetPosition(x, y));
	EXPECT_THROW(music.SetPosition(x, y), std::runtime_error);
}

TEST_F(AudioTest, MinDistance) {
    float distance = 15.0f;
    EXPECT_NO_THROW(sound.SetMinDistance(distance));
	EXPECT_THROW(music.SetMinDistance(distance), std::runtime_error);
}

TEST_F(AudioTest, Attenuation) {
    float attenuation = 0.5f;
    EXPECT_NO_THROW(sound.SetAttenuation(attenuation));
	EXPECT_THROW(music.SetAttenuation(attenuation), std::runtime_error);
}

TEST_F(AudioTest, Duration) {
    EXPECT_GT(sound.GetDuration(), 0);
    EXPECT_GT(music.GetDuration(), 0);
}

TEST_F(AudioTest, CsvString) {
    std::string soundCsv = sound.GetCsvString();
    std::string musicCsv = music.GetCsvString();

    EXPECT_NE(soundCsv.find("Sound Test"), std::string::npos);
    EXPECT_NE(musicCsv.find("Music Test"), std::string::npos);
}
