#pragma once

#include <ImGui.h>
#include <icons.h>
#include <iostream>

#include "Utility/Style.h"
#include "Utility/FileDialoge.h"
#include "GUI/Property/UIBase.h"
#include "GUI/UIUtility/UIUtility.h"
#include "Sprite/Sprite.h"
#include "GUI/Color.h"
#include "Savesystem.h"
#include "Input/Input.h"
#include "GUI/Property/Animations/UIAnimationEditor.h"

#define WINDOW_SIZE_ANIMATION_CREATE ImVec2(500, 500)
#define START_CNT_BG 1

namespace spe
{
	class UIAnimation : public spe::IUIBase
	{
	private:
		//Setting display to false and shit in its own class
		spe::UIAnimationEditor m_UIAnimationEditor;
		std::string m_FileName;

		spe::FileDialog m_OpenFileDialoge;
		spe::FileDialog m_CreateFileDialoge;

		ImGuiTextFilter m_FilterSearch;

		uint8_t m_BackgroundCounter;
		char m_AnimationFile[150];

		void GetFileNameInput();
		void DisplayAnimations();
		void AddAnimationsToAnimator();
		void EnterAnimation(spe::Animation& animation);
		void DrawBackgroundBehinAnimation();
		void RenderAnimationUIOptions() noexcept;

		void Init() override;
	public:
		UIAnimation();

		void Render() override;
	};
}