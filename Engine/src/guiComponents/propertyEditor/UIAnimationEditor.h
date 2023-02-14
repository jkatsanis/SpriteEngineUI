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
		const int m_CURSOR_SPACE = 50;
		int m_keyFramesToEdit;
		UIAnimationKeyFrameEditor editor;

		void renderKeyFrames();
		void addKeyFrame();
		void beginWindow();
		void closeWindow();
		void editorTimeLine();

		s2d::Animation* m_anim;

	public:
		bool isHovered;
		bool display;

		void setAnim(s2d::Animation* anim);
		void resetAnim();

		UIAnimationEditor();

		void displayEditor();
	};
}
