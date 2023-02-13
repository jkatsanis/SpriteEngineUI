#include "Animator.h"
#include <physicalComponents/sprite.h>

#define EXIST if (!this->exists) return;
	
s2d::Animator::Animator()
{
	this->exists = false;
	this->ptr_attachedSprite = nullptr;
}

s2d::Animator::Animator(Sprite* ptr_attachedSprite)
{
	this->exists = false;
	this->ptr_attachedSprite = ptr_attachedSprite;
	this->animationPlaying.isAAnimationPlaying = false;
	this->animationPlaying.name = "<Unknown>";
}

void s2d::Animator::createAnimation(const std::string& name, const std::vector<s2d::KeyFrame>& textures, const float delay, const bool useBaseSprite)
{
	EXIST
	animations.push_back(Animation(ptr_attachedSprite, name, textures, delay, useBaseSprite));
}

void s2d::Animator::play(std::string name)
{
	EXIST
	for (Animation& anim : animations)
	{
		if (anim.name == name && !this->animationPlaying.isAAnimationPlaying)
		{
			this->animationPlaying.name = name;
			this->animationPlaying.isAAnimationPlaying = true;
			anim.play();
		}
	}
}

void s2d::Animator::stop(std::string name)
{
	EXIST;
	for (Animation& anim : animations)
	{
		if (anim.name == name && anim.isPlaying)
		{
			this->animationPlaying.name = "<Unknown>";
			this->animationPlaying.isAAnimationPlaying = false;
			anim.stop();
		}
	}
}

void s2d::Animator::update()
{
	EXIST;	

	for (Animation& anim : this->ptr_attachedSprite->animator.animations)
	{
		if (anim.isPlaying)
			anim.update();
	}
}

void s2d::Animator::resetComponent()
{
	this->animationPlaying.name = "<Unknown>";;
	this->animationPlaying.isAAnimationPlaying = false;
	this->exists = false;
	this->animations.clear();
}

//Static functions

void s2d::Animator::stopAllAnimations()
{
	for (int i = 0; i < s2d::Sprite::activeSprites.size(); i++)
	{
		s2d::Sprite::activeSprites[i]->animator.stop(s2d::Sprite::activeSprites[i]->animator.animationPlaying.name);
	}
}
