#pragma once

#include <iostream>	
#include <vector>
#include <spriteComponents/animation.h>
#include <engineComponents/stime.h>
#include <spriteComponents/keyFrame.h>
#include <manager/spriteRepository.h>
#include <unordered_map>

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
		std::unordered_map<std::string, Animation> animations;

		Animator();
		Animator(Sprite* ptr_attachedSprite);

		void createAnimation(const std::string& name, const std::string& fileLocation, const std::vector<s2d::KeyFrame>& frame);
		void removeAnimation(const std::string& name);

		void play(const std::string& name);
		void stop(const std::string& name);

		void update();

		void resetComponent();

	public:

		static void stopAllAnimations(s2d::SpriteRepository& toUpdate);
	};
}

