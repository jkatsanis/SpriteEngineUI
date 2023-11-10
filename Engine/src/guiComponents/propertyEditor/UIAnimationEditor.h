#pragma once

#include <spriteComponents/animation.h>
#include <ImGui.h>
#include <data/UIInfo.h>
#include <icons.h>
#include <imgui_internal.h>
#include <UIAnimationKeyFrameAdder.h>
#include <guiComponents/colors.h>

#define WINDOW_SIZE_ANIMATION_EDITOR ImVec2(900, 350)

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
		const float MIN_CURSOR_SPACE = 2.0f;
		const float MAX_CURSOR_SPACE = 10;
		const float SMALL_INCREMENT = 0.5f;
		const float LARGE_INCREMENT = 2;

		char m_keyFramePathDataHolder[100];
		int m_key_frame_pos;
		std::string m_key_frame_path;

		sf::Event* m_ptr_event_engine;
		float m_cursor_space;
		int m_keyFramesToEdit;
		s2d::SpriteRepository* m_ptr_repo;
		s2d::UIAnimationKeyFrameAdder keyFrameAdder;
		 
		void renderTimeLineRealTimePoint();
		void zoomEditorTimeLine();
		void renderKeyFrames();
		void addKeyFrame();
		void beginWindow();
		void closeWindow();
		void editorTimeLine();
		void displayKeyFrameInfo();
		bool renderTextBasedOnScroll(size_t i);
		void saveAnimation();
		void renameAnimation();

		bool displayTimeFrameBasedOnCursorSpace(size_t i_pos);

		s2d::Animation* m_anim;
		s2d::KeyFrameSelected m_keyFrameSelected;
	public:
		bool is_hovered;
		bool display;

		void setAnim(s2d::Animation* anim);
		void setEvent(sf::Event* event) { this->m_ptr_event_engine = event; }
		void resetAnim();

		UIAnimationEditor();

		void displayEditor();
		void setSpriteRepository(s2d::SpriteRepository& repo);
	};
}
