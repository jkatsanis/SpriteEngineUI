#pragma once

#include <spriteComponents/animation.h>
#include <ImGui.h>
#include <data/UIInfo.h>
#include <icons.h>
#include <imgui_internal.h>
#include <UIAnimationKeyFrameAdder.h>
#include <guiComponents/colors.h>

#define CURSOR_SPACE 50

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
		int m_keyFramesToEdit;
		s2d::SpriteRepository* m_ptr_repo;
		s2d::UIAnimationKeyFrameAdder keyFrameAdder;

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
		void setSpriteRepository(s2d::SpriteRepository& repo) { this->m_ptr_repo = &repo; }
	};
}
