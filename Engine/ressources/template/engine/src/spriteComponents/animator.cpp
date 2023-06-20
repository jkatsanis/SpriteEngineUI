#include "Animator.h"
#include <physicalComponents/sprite.h>

#define EXIST if (!this->exist) return

// Constructor / Destructor

s2d::Animator::Animator()
{
	this->init();
}

s2d::Animator::Animator(Sprite* ptr_attached_sprite)
{
	this->init();
	this->ptr_attached_sprite = ptr_attached_sprite;
}

s2d::Animator::Animator(s2d::Sprite* ptr_attached_sprite, s2d::Animator& animator)
{
	this->init();
	this->exist = animator.exist;
	this->ptr_attached_sprite = ptr_attached_sprite;

	for (const auto& animation : animator.animations)
	{
		// Copy animation
		s2d::Animation anim = s2d::Animation(this->ptr_attached_sprite, animation.second);
		animations.insert({ anim.name, anim });
	}
}

void s2d::Animator::init()
{
	this->exist = false;
	this->ptr_attached_sprite = nullptr;
	this->animationPlaying.isAAnimationPlaying = false;
	this->animationPlaying.name = "<Unknown>";
}

// Public functions

void s2d::Animator::createAnimation(const std::string& name, const std::string& fileLocation, const std::vector<s2d::KeyFrame>& textures)
{
	EXIST;
	animations.insert({ name, Animation(ptr_attached_sprite, name, fileLocation, textures) });
}

void s2d::Animator::removeAnimation(const std::string& name)
{
	EXIST;
	animations.erase(name);
}

void s2d::Animator::play(const std::string& name)
{
	EXIST;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		this->animationPlaying.name = name;
		this->animationPlaying.isAAnimationPlaying = true;
		it->second.play();
	}
}

void s2d::Animator::stop(const std::string& name)
{
	EXIST;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		this->animationPlaying.name = name;
		this->animationPlaying.isAAnimationPlaying = true;
		it->second.stop();
	}
}

void s2d::Animator::update()
{
	EXIST;
	for (auto& anim : this->animations)
	{
		if (anim.second.is_playing)
			anim.second.update();
	}
}

void s2d::Animator::reset()
{
	this->animationPlaying.name = "<Unknown>";;
	this->animationPlaying.isAAnimationPlaying = false;
	this->exist = false;
	this->animations.clear();
}

//Static functions

void s2d::Animator::stopAllAnimations(s2d::SpriteRepository& toUpdate)
{
	for (int i = 0; i < toUpdate.amount(); i++)
	{
		s2d::Sprite* const sprite = toUpdate.readAt(i);
		sprite->animator.stop(sprite->animator.animationPlaying.name);
	}
}
