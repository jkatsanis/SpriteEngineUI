#pragma once

#include <imgui.h>

#include "GUI/Property/UIBase.h"
#include "GUI/UIUtility/UIUtility.h"
#include "GUI/Color.h"
#include "GUI/Property/Animations/UIAnimationKeyFrameAdder.h"

#include "Sprite/Sprite.h"
#include "Savesystem.h"
#include "Utility/FileDialoge.h"

#define WINDOW_SIZE_AUDIO_EDITOR ImVec2(600, 600)

namespace spe
{
    class UIAudioEditor : public spe::IUIBase
    {
    private:
        const float MIN_CURSOR_SPACE = 2.0f;
        const float MAX_CURSOR_SPACE = 10;
        float m_CursorSpace;

        char m_AudioNameBuffer[256];
        int m_SelectedAudioType;
        bool m_IsPlaying;

        // Temporary settings that are only applied on save
        char m_TempAudioPath[1024];
        float m_TempVolume;
        float m_TempPitch;
        bool m_TempPlayOnStart;
        bool m_TempLoop;

        // Sound-specific settings
        bool m_TempRelativeToListener;
        float m_TempPositionX;
        float m_TempPositionY;
        float m_TempMinDistance;
        float m_TempAttenuation;

        // Music-specific settings
        float m_TempLoopPointOffset;
        float m_TempLoopPointLength;

        void Init() override;
        void LoadSettingsFromAudio();
        void ApplySettingsToAudio();
    public:
        bool Display;
        spe::FileDialog m_OpenFileDialog;
        const char * m_FileName;

        UIAudioEditor();
        void Render() override;

        void ResetAudio();
    };
}
