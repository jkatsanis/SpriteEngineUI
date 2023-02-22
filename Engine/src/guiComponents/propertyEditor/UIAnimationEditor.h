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
	struct KeyFrameSelected
	{
		int position;
		const s2d::KeyFrame* keyFrameSelected;
		bool isClicked;
	};
	class UIAnimationEditor
	{
	private:
		const int m_CURSOR_SPACE = 50;
		int m_keyFramesToEdit;
		s2d::UIAnimationKeyFrameEditor editor;

		void renderKeyFrames();
		void addKeyFrame();
		void beginWindow();
		void closeWindow();
		void editorTimeLine();
		void displayKeyFrameInfo();

		s2d::Animation* m_anim;
		s2d::KeyFrameSelected m_keyFrameSelected;
	public:
		bool isHovered;
		bool display;

		void setAnim(s2d::Animation* anim);
		void resetAnim();

		UIAnimationEditor();

		void displayEditor();
	};
}
