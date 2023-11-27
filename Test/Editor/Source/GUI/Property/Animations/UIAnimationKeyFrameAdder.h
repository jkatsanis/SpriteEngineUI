#pragma once

#include <iostream>

#include "Components/Animator/Animation.h"
#include "GUI/Property/UIBase.h"

#include "GUI/UIUtility/UIUtility.h"


namespace spe
{
	class UIAnimationKeyFrameAdder : public spe::IUIBase
	{
	private:
		spe::Animation* m_animation;

		void beginWindow();
		void inputData();
		void closeWindowAndSafeKeyFrame();
		void addKeyFrameToAnimation();

		int m_key_frame_pos;
		char m_keyFramePathDataHolder[100];	// 100 is the path array size (string can only be 100 chars long!)

		void Init() override;
	public:
		bool is_key_frame_menu_open;
		bool is_hovered;
		std::string key_frame_path;

		void Render() override;
		void setAnimation(spe::Animation* anim);
		void reset();
	};
}

