#pragma once

#include <ImGui.h>
#include <UIInfo.h>
#include <icons.h>
#include <iostream>
#include <sprite.h>
#include <UIHirachy.h>
#include <UIAnimationEditor.h>
#include <fontManager.h>

namespace s2d
{
	class UIAnimation
	{
	private:
		//Setting display to false and shit in its own class
		UIAnimationEditor m_UIAnimationEditor;
		std::string m_current_sprite;
		std::string m_file_name;

		char mogus[150];

		void getFileNameInput();
		void displayAnimations();
		void addAnimationsToAnimator();
		void displayTopOfEditor();

	public:
		bool isHovered;

		UIAnimation();

		void createUIAnimationWindow();
	};
}