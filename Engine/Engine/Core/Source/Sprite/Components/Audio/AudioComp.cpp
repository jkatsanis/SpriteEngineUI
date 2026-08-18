//
// Created by toaster on 10/16/25.
//

#include "AudioComp.h"

void spe::AudioComp::Init()
{
    this->BaseComponent = false;
    this->Exist = false;
    this->ptr_Sprite = nullptr;
    this->Audios = std::vector<Audio*>(0);
}

spe::AudioComp::AudioComp()
{
    this->AudioComp::Init();
}

spe::AudioComp::AudioComp(Sprite* spr)
{
    this->AudioComp::Init();
    this->ptr_Sprite = spr;
}

spe::AudioComp::AudioComp(Sprite* spr, const AudioComp& comp) {
    this->AudioComp::Init();
    this->ptr_Sprite = spr;
    this->Exist = comp.Exist;
    this->Audios = comp.Audios;
}

void spe::AudioComp::Reset()
{
    Audios.clear();
}

void spe::AudioComp::AddAudio(Audio* audio) {
    for (const auto* a : Audios) {
        if (a->GetName() == audio->GetName()) {
            spe::Log::LogString("AudioComp::AddAudio - Audio with name " + audio->GetName() + " already exists.");
            return;
        }
    }
    Audios.push_back(audio);
}

void spe::AudioComp::RemoveAudio(const std::string& name) {
    for (size_t i = 0; i < Audios.size(); i++) {
        if (Audios[i]->GetName() == name) {
            Audios.erase(Audios.begin() + i);
            return;
        }
    }
}

void spe::AudioComp::PlayAllStartAudios() const {
    for (auto &audio : Audios) {
        if (audio->PlayOnStart) {
            audio->Load();
            audio->Play();
        }
    }
}

std::vector<spe::AudioInfo> spe::AudioComp::GetAllAudiosInfo() const {
    std::vector<AudioInfo> infos;
    infos.reserve(Audios.size());
    for (const auto* audio : Audios) {
        infos.push_back({ audio->GetName(), audio->GetPath(), audio->GetType() == Audio::AudioType::MUSIC? "MUSIC" : "SOUND" });
    }
    return infos;
}

spe::Audio* spe::AudioComp::GetAudioByName(const std::string& name) const {
    for (auto* audio : Audios) {
        if (audio->GetName() == name) {
            return audio;
        }
    }
    spe::Log::LogString("AudioComp::GetAudioByName - Audio with name " + name + " not found.");
    return nullptr;
}