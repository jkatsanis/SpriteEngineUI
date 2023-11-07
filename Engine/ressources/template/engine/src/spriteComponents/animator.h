
#pragma once

#include <spriteComponents/animation.h>
#include <engineComponents/stime.h>
#include <spriteComponents/keyFrame.h>
#include <manager/spriteRepository.h>
#include <spriteComponents/component.h>

#include <iostream>	
#include <vector>
#include <unordered_map>

namespace s2d
{
	struct AnimationPlaying
	{
		std::string name;
		bool isAAnimationPlaying;
	};

	class Sprite;
	class Animator : public s2d::Component
	{
	private:
		void init() override;
	public:
		AnimationPlaying animationPlaying;
		Sprite* ptr_attached_sprite;
		std::unordered_map<std::string, Animation> animations;

		Animator();
		Animator(s2d::Sprite* ptr_attached_sprite);
		Animator(s2d::Sprite* ptr_attached_sprite, s2d::Animator& animator);

		void createAnimation(const std::string& name, const std::string& fileLocation, const std::vector<s2d::KeyFrame>& frame);
		void removeAnimation(const std::string& name);

		void play(const std::string& name);
		void stop(const std::string& name);

		void stopAll();

		void update();

		void reset() override;

	public:

		static void stopAllAnimations(s2d::SpriteRepository& toUpdate);
	};
}