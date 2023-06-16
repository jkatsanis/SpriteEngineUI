#pragma once

#include <ImGui.h>
#include <data/UIInfo.h>
#include <icons.h>
#include <iostream>
#include <physicalComponents/sprite.h>
#include <UIAnimationEditor.h>
#include <manager/fontManager.h>
#include <manager/fileDialogs.h>
#include <engineComponents/input.h>

#define WINDOW_SIZE_ANIMATION_CREATE ImVec2(500, 500)
#define START_CNT_BG 1

namespace s2d
{
	class UIAnimation
	{
	private:
		//Setting display to false and shit in its own class
		s2d::SpriteRepository* m_ptr_repo;
		UIAnimationEditor m_UIAnimationEditor;
		std::string m_fileName;
		s2d::FileDialog m_createAnimtionPathFileDialoge;
		ImGuiTextFilter m_search_filter_animation;

		uint8_t m_background_counter;
		bool m_openFileDialog;
		bool m_openInputWindow;
		char m_animationFile[150];

		void getFileNameInput();
		void displayAnimations();
		void addAnimationsToAnimator();
		void enterAnimation(s2d::Animation& animation);
		void drawBackgroundBehinAnimation();
		void renderAnimationUIOptions();

	public:
		bool is_hovered;

		UIAnimation();

		void createUIAnimationWindow();
		void setSpriteRepository(s2d::SpriteRepository& repo);
	};
}