#pragma once

#include <ImGui/ImGUISFML.h>

#include "GUI/Property/UIBase.h"
#include "GUI/UIUtility/UIUtility.h"
#include "GUI/Color.h"
#include "GUI/Property/Animations/UIAnimationKeyFrameAdder.h"

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

		char m_KeyFramePath[100];
		int m_KeyFramePos;
		std::string m_KeyFramePath;

		float m_CursorSpace;
		int m_KeyFramesToEdit;
		spe::UIAnimationKeyFrameAdder m_FrameAdder;
		 
		void RenderTimeLineRealTimePoint();
		void ZoomEditorTimeLine();
		void RenderKeyFrames();
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
		void resetAnim();

		void Render() override;
	};
}
