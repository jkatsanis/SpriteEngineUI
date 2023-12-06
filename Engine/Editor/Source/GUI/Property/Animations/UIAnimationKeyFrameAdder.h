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
		spe::Animation* m_ptr_Anim;

		void BeginWindow();
		void InputData();
		void CloseWindowAndSafeKeyFrame();
		void AddKeyFrameToAnimation();

		int m_KeyFramePos;
		char m_KeyFramePath[100];	// 100 is the path array size (string can only be 100 chars long!)

		void Init() override;
	public:
		bool IsKeyFrameMenuOpen;
		std::string KeyFramePath;

		UIAnimationKeyFrameAdder();

		void Render() override;
		void SetAnimation(spe::Animation* anim);
		void Reset();
	};
}

