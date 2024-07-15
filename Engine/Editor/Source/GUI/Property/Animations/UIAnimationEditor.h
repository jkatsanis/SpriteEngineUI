#pragma once

#include <imgui.h>

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

		int m_KeyFrameSelectedPos;

		int m_KeyFramePos;
		std::string m_KeyFramePathStd;

		float m_CursorSpace;
		int m_KeyFramesToEdit;
		spe::UIAnimationKeyFrameAdder m_FrameAdder;
		 
		void RenderTimeLineRealTimePoint();
		void ZoomEditorTimeLine();
		void RenderKeyFrames();
		void AddKeyFrame();
		void BeginWindow();
		void CloseWindow();
		void EditorTimeLine();
		void DisplayKeyFrameInfo();
		bool RenderTextBasedOnScroll(size_t i);
		void SaveAnimation();
		void RenameAnimation();

		bool DisplayTimeFrameBasedOnCursorSpace(size_t i_pos);

		spe::Animation* m_ptr_Anim;
		spe::KeyFrameSelected m_KeyFrameSelected;

		void Init() override;
	public:
		bool Display;

		UIAnimationEditor();

		void SetAnim(spe::Animation* anim);
		void ResetAnim();

		void Render() override;
	};
}
