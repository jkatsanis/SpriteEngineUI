#pragma once

#include <iostream>
#include <UIAssetFolder.h>
#include <UIInspector.h>
#include <spriteComponents/animation.h>
#include <manager/fontManager.h>

namespace s2d
{
	class UIAnimationKeyFrameAdder
	{
	private:
		s2d::Animation* m_animation;
		s2d::SpriteRepository* m_ptr_repo;

		void beginWindow();
		void inputData();
		void closeWindowAndSafeKeyFrame();
		void addKeyFrameToAnimation();

		int m_keyframePos;
		char m_keyFramePathDataHolder[100];	// 100 is the path array size (string can only be 100 chars long!)

	public:
		bool isKeyFrameMenuOpen;
		bool isHovered;
		std::string keyFramePath;

		UIAnimationKeyFrameAdder();

		void update();
		void setAnimation(s2d::Animation* anim);
		void reset();

		void setSpriteRepository(s2d::SpriteRepository& repo) { this->m_ptr_repo = &repo; }
	};
}

