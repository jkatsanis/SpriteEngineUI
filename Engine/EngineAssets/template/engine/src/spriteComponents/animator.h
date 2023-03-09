#pragma once

#include <iostream>	
#include <vector>
#include <spriteComponents/animation.h>
#include <engineComponents/stime.h>
#include <spriteComponents/keyFrame.h>

namespace s2d
{
	struct AnimationPlaying
	{
		std::string name;
		bool isAAnimationPlaying;
	};

	class Sprite;
	class Animator
	{
	public:
		AnimationPlaying animationPlaying;
		bool exists;
		Sprite* ptr_attachedSprite;
		std::vector<Animation> animations;

		Animator();
		Animator(Sprite* ptr_attachedSprite);

		void createAnimation(const std::string& name, const std::vector<s2d::KeyFrame>& frame);

		void play(std::string name);
		void stop(std::string name);

		void update();

		void resetComponent();

	public:

		static void stopAllAnimations();
	};
}

