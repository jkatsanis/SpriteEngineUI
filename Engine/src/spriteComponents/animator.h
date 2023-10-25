#pragma once

#include <iostream>	
#include <vector>
#include <spriteComponents/animation.h>
#include <engineComponents/stime.h>
#include <spriteComponents/keyFrame.h>
#include <manager/spriteRepository.h>
#include <map>
#include <spriteComponents/component.h>

#define EXIST_COMPONENT if (!this->exist) return

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
		AnimationPlaying m_animation_playing;

		void init() override;
	public:
		Sprite* ptr_attached_sprite;
		std::map<std::string, Animation> animations;

		Animator();
		Animator(Sprite* ptr_attachedSprite);
		Animator(s2d::Sprite* ptr_attached_sprite, s2d::Animator& animator);

		void createAnimation(const std::string& name, const std::string& fileLocation, const std::vector<s2d::KeyFrame>& frame);
		void removeAnimation(const std::string& name);

		void play(const std::string& name);
		void stop(const std::string& name);

		void setName(const std::string& new_name, const std::string& old_name);
		void update();

		void reset() override;

	public:

		static void stopAllAnimations(s2d::SpriteRepository& toUpdate);
	};
}

