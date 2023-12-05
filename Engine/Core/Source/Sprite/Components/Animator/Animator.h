#pragma once

#include <iostream>	
#include <vector>

#include "Sprite/Components/Animator/Animation.h"
#include "Sprite/Components/Component.h"



#define EXIST_COMPONENT if (!this->exist) return

namespace spe
{
	struct AnimationPlaying
	{
		std::string Name;
		bool IsPlaying;
	};

	class Sprite;
	class Animator : public spe::Component
	{
	private: 
		AnimationPlaying m_AnimationPlaying;

		void init() override;
	public:
		Sprite* ptr_attached_sprite;
		std::map<std::string, Animation> Animations;

		Animator();
		Animator(Sprite* ptr_attachedSprite);
		Animator(spe::Sprite* ptr_attached_sprite, spe::Animator& animator);

		void CreateAnimation(const std::string& name, const std::string& fileLocation, const std::vector<spe::KeyFrame>& frame);
		void RemoveAnimation(const std::string& name);

		void Play(const std::string& name);
		void Stop(const std::string& name);

		void SetName(const std::string& new_name, const std::string& old_name);
		void Update();

		void reset() override;
	};
}

