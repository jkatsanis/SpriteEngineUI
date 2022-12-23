#pragma once

#include <iostream>	
#include <vector>
#include <animation.h>
#include <stime.h>

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

		/*
		  * @brief Playing while it executes wont affect anything, you need to stop the animationd and play it then
		  * @param name the animation it should play
		*/
		void play(std::string name);
		void stop(std::string name);

		void update();

		void resetComponent();
	public:

		/*
		  * @brief Will stop all animations of all sprites 
		*/
		static void stopAllAnimations();
	};
}

