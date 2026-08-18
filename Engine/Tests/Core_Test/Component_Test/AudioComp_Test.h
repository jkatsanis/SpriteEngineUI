#pragma once

#include <gtest/gtest.h>
#include "Sprite/Components/Audio/AudioComp.h"
#include "Audio/Audio.h"
#include <memory>
#include <vector>

class AudioCompTest : public ::testing::Test {
protected:
    spe::AudioComp m_comp;
    const std::string audioPath = "Tests/Core_Test/TestResources/audio.wav";

    std::vector<std::unique_ptr<spe::Audio>> managedAudios;

    spe::Audio* CreateManagedAudio(const std::string& name, const std::string& path, spe::Audio::AudioType type) {
        auto audio = std::make_unique<spe::Audio>(path, type, name);
        spe::Audio* ptr = audio.get();
        managedAudios.push_back(std::move(audio));
        return ptr;
    }

    void TearDown() override {
        m_comp.Reset();
        managedAudios.clear();
    }
};

TEST_F(AudioCompTest, DefaultConstructor) {
    EXPECT_EQ(m_comp.Audios.size(), 0);
    EXPECT_FALSE(m_comp.Exist);
}

TEST_F(AudioCompTest, AddAudio) {
    spe::Audio* audio = CreateManagedAudio("TestSound", audioPath, spe::Audio::AudioType::SOUND);
    m_comp.AddAudio(audio);

    EXPECT_EQ(m_comp.Audios.size(), 1);
    EXPECT_EQ(m_comp.GetAudioByName("TestSound"), audio);

    EXPECT_FALSE(m_comp.Exist);
}

TEST_F(AudioCompTest, RemoveAudio) {
    spe::Audio* audio = CreateManagedAudio("Sound1", audioPath, spe::Audio::AudioType::SOUND);
    m_comp.AddAudio(audio);

    ASSERT_EQ(m_comp.Audios.size(), 1);

    m_comp.RemoveAudio("Sound1");
    EXPECT_EQ(m_comp.Audios.size(), 0);
}

TEST_F(AudioCompTest, GetAudioByName) {
    spe::Audio* audio = CreateManagedAudio("Sound1", audioPath, spe::Audio::AudioType::SOUND);
    m_comp.AddAudio(audio);

    EXPECT_EQ(m_comp.GetAudioByName("Sound1"), audio);
    EXPECT_EQ(m_comp.GetAudioByName("NonExistent"), nullptr);
}

TEST_F(AudioCompTest, GetAllAudiosInfo) {
    m_comp.AddAudio(CreateManagedAudio("Sound1", audioPath, spe::Audio::AudioType::SOUND));
    m_comp.AddAudio(CreateManagedAudio("Music1", audioPath, spe::Audio::AudioType::MUSIC));

    auto infos = m_comp.GetAllAudiosInfo();
    ASSERT_EQ(infos.size(), 2);

    bool foundSound1 = false;
    bool foundMusic1 = false;
    for (const auto& info : infos) {
        if (info.Name == "Sound1") {
            EXPECT_EQ(info.Path, audioPath);
            EXPECT_EQ(info.Type, "SOUND");
            foundSound1 = true;
        } else if (info.Name == "Music1") {
            EXPECT_EQ(info.Path, audioPath);
            EXPECT_EQ(info.Type, "MUSIC");
            foundMusic1 = true;
        }
    }
    EXPECT_TRUE(foundSound1) << "Audio info for 'Sound1' not found.";
    EXPECT_TRUE(foundMusic1) << "Audio info for 'Music1' not found.";
}

TEST_F(AudioCompTest, EdgeCase_RemoveNonExistent) {
    m_comp.AddAudio(CreateManagedAudio("Sound1", audioPath, spe::Audio::AudioType::SOUND));

    m_comp.RemoveAudio("NonExistent");
    EXPECT_EQ(m_comp.Audios.size(), 1);
}

TEST_F(AudioCompTest, EdgeCase_AddDuplicateName) {
    m_comp.AddAudio(CreateManagedAudio("Sound1", audioPath, spe::Audio::AudioType::SOUND));
    m_comp.AddAudio(CreateManagedAudio("Sound1", audioPath, spe::Audio::AudioType::SOUND));

    EXPECT_EQ(m_comp.Audios.size(), 1);

    // we can still retrieve original:
    EXPECT_NE(m_comp.GetAudioByName("Sound1"), nullptr);
}

TEST_F(AudioCompTest, CopyConstructor) {
    m_comp.Exist = true;
    m_comp.AddAudio(CreateManagedAudio("Sound1", audioPath, spe::Audio::AudioType::SOUND));

    spe::AudioComp copy(nullptr, m_comp);

    EXPECT_TRUE(copy.Exist);
    EXPECT_EQ(copy.Audios.size(), 1);
    EXPECT_EQ(copy.Audios[0]->GetName(), "Sound1");
}

TEST_F(AudioCompTest, Reset) {
    m_comp.AddAudio(CreateManagedAudio("Sound1", audioPath, spe::Audio::AudioType::SOUND));
    m_comp.Reset();
    EXPECT_EQ(m_comp.Audios.size(), 0);
}
