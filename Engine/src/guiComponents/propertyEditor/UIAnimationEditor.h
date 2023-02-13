#pragma once

#include <spriteComponents/animation.h>
#include <ImGui.h>
#include <data/UIInfo.h>
#include <icons.h>
#include <imgui_internal.h>
#include <UIAnimationKeyFrameEditor.h>
#include <guiComponents/colors.h>

namespace s2d
{
	class UIAnimationEditor
	{
	private:
		int m_keyFramesToEdit;
		UIAnimationKeyFrameEditor editor;

		void addKeyFrame();
		void beginWindow();
		void closeWindow();
		void editorTimeLine();

	public:
		bool isHovered;
		bool display;
		s2d::Animation* anim;

		UIAnimationEditor();

		void displayEditor();
	};
}
