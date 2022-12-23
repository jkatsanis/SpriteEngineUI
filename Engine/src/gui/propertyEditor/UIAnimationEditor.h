#pragma once

#include <animation.h>
#include <ImGui.h>
#include <UIInfo.h>
#include <icons.h>
#include <imgui_internal.h>
#include <UIAnimationKeyFrameEditor.h>

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
		void settings();

	public:
		bool isHovered;
		bool display;
		s2d::Animation* anim;

		UIAnimationEditor();

		void displayEditor();
	};
}
