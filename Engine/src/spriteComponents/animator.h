#pragma once

#include <iostream>	
#include <vector>
#include <spriteComponents/animation.h>
#include <engineComponents/stime.h>

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

		void createAnimation(std::string name, std::vector<std::string> textures, float delay, bool useBaseSprite);

		void play(std::string name);
		void stop(std::string name);

		void update();

		void resetComponent();

	public:

		static void stopAllAnimations();
	};
}

