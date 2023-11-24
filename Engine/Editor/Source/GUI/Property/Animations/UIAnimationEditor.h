#pragma once

#include <ImGui/ImGUISFML.h>

#include "GUI/Property/UIBase.h"
#include "GUI/UIUtility/UIUtility.h"
#include "GUI/Color.h"

#include "Sprite/Sprite.h"
#include "Savesystem.h"

#define WINDOW_SIZE_ANIMATION_EDITOR ImVec2(900, 350)

namespace spe
{
	class UIAnimationEditor : public spe::IUIBase
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
		spe::SpriteRepository* m_ptr_repo;
		// spe::UIAnimationKeyFrameAdder keyFrameAdder;
		 
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

		spe::Animation* m_anim;
		spe::KeyFrameSelected m_keyFrameSelected;

		void Init() override;
	public:
		bool display;

		void setAnim(spe::Animation* anim);
		void setEvent(sf::Event* event) { this->m_ptr_event_engine = event; }
		void resetAnim();

		void Render() override;
	};
}
