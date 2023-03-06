#pragma once

#include <ImGui.h>
#include <data/UIInfo.h>
#include <icons.h>
#include <iostream>
#include <physicalComponents/sprite.h>
#include <UIHirachy.h>
#include <UIAnimationEditor.h>
#include <manager/fontManager.h>
#include <manager/fileDialogs.h>

namespace s2d
{
	class UIAnimation
	{
	private:
		//Setting display to false and shit in its own class
		UIAnimationEditor m_UIAnimationEditor;
		std::string m_current_sprite;
		std::string m_file_name;
		s2d::FileDialog m_createAnimtionPathFileDialoge;

		bool m_openFileDialog;
		bool m_openInputWindow;
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