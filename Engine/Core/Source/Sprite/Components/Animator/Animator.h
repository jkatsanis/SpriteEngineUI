#pragma once

#include <iostream>	
#include <vector>

#include "Animation.h"
#include "Components/Component.h"



#define EXIST_COMPONENT if (!this->exist) return

namespace spe
{
	struct AnimationPlaying
	{
		std::string name;
		bool isAAnimationPlaying;
	};

	class Sprite;
	class Animator : public spe::Component
	{
	private: 
		AnimationPlaying m_animation_playing;

		void init() override;
	public:
		Sprite* ptr_attached_sprite;
		std::map<std::string, Animation> animations;

		Animator();
		Animator(Sprite* ptr_attachedSprite);
		Animator(spe::Sprite* ptr_attached_sprite, spe::Animator& animator);

		void createAnimation(const std::string& name, const std::string& fileLocation, const std::vector<spe::KeyFrame>& frame);
		void removeAnimation(const std::string& name);

		void play(const std::string& name);
		void stop(const std::string& name);

		void setName(const std::string& new_name, const std::string& old_name);
		void update();

		void reset() override;

	public:

		// static void stopAllAnimations(spe::SpriteRepository& toUpdate);
	};
}

