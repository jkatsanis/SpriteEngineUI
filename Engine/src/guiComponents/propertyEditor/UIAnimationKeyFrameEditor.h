#pragma once

#include <iostream>
#include <UIAssetFolder.h>
#include <UIInspector.h>
#include <spriteComponents/animation.h>
#include <manager/fontManager.h>

namespace s2d
{
	class UIAnimationKeyFrameEditor
	{
	private:
		s2d::Animation* m_animation;

		void beginWindow();
		void inputData();
		void closeWindowAndSafeKeyFrame();
		void addKeyFrameToAnimation();

	public:
		bool isKeyFrameMenuOpen;
		std::string keyFramePath;
		char keyFramePathArr[100];	// 100 is the path array size (string can only be 100 chars long!)
		bool isHovered;
		int pos;

		UIAnimationKeyFrameEditor();

		void update();
		void setAnimation(s2d::Animation* anim);
	};
}

