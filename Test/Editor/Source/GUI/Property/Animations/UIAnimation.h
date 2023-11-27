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
		std::string m_fileName;

		spe::FileDialog m_animation_open_file_dialog;
		spe::FileDialog m_animation_create_file_dialog;

		ImGuiTextFilter m_search_filter_animation;

		uint8_t m_background_counter;
		char m_animationFile[150];

		void getFileNameInput();
		void displayAnimations();
		void addAnimationsToAnimator();
		void enterAnimation(spe::Animation& animation);
		void drawBackgroundBehinAnimation();
		void renderAnimationUIOptions();

		void Init() override;
	public:

		void Render() override;
	};
}