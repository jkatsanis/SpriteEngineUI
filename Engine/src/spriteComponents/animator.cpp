#include "Animator.h"
#include <physicalComponents/sprite.h>

#define EXIST if (!this->exists) return
	
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

void s2d::Animator::createAnimation(const std::string& name, const std::string& fileLocation, const std::vector<s2d::KeyFrame>& textures)
{
	EXIST;
	animations.insert( { name, Animation(ptr_attachedSprite, name, fileLocation, textures) });
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
		if (anim.second.isPlaying)
			anim.second.update();
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
	for (int i = 0; i < s2d::Sprite::s_sprites.size(); i++)
	{
		s2d::Sprite::s_sprites[i]->animator.stop(s2d::Sprite::s_sprites[i]->animator.animationPlaying.name);
	}
}
