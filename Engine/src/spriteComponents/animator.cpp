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
}

void s2d::Animator::createAnimation(std::string name, std::vector<std::string> textures, float delay, bool useBaseSprite)
{
	EXIST;
	std::cout << "create";
	animations.push_back(Animation(ptr_attachedSprite, name, textures, delay, useBaseSprite));
}

void s2d::Animator::play(std::string name)
{
	EXIST;	std::cout << "play";

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
	std::cout << "stop";
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
	this->animationPlaying.name = "";
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
