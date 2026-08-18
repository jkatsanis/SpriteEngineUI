#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <math.h>

#include "Math/Vector2.h"
#include "Sprite/Components/Component.h"
#include "Sprite/Components/Light/LightRepository.h"
#include "Camera/Camera.h"
#include "Audio/Audio.h"


namespace spe
{
    struct AudioInfo {
        std::string Name;
        std::string Path;
        std::string Type;
    };

    class AudioComp : public spe::Component
    {
    private:
        Sprite * ptr_Sprite;

        void Init() override;

    public:
        AudioComp();
        AudioComp(spe::Sprite* spr);
        AudioComp(Sprite *spr, const AudioComp &comp);

        std::vector<spe::Audio*> Audios;

        void Reset() override;

        void AddAudio(Audio *audio);

        void RemoveAudio(const std::string &name);

        void PlayAllStartAudios() const;

        std::vector<spe::AudioInfo> GetAllAudiosInfo() const;

        spe::Audio *GetAudioByName(const std::string &name) const;
    };
}


