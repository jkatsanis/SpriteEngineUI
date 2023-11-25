#include "Animator.h"

#include "Sprite.h"

// Constructor / Destructor

spe::Animator::Animator()
{
	this->init();
}

spe::Animator::Animator(Sprite* ptr_attached_sprite)
{
	this->init();
	this->ptr_attached_sprite = ptr_attached_sprite;
}

spe::Animator::Animator(spe::Sprite* ptr_attached_sprite, spe::Animator& animator)
{
	this->init();
	this->exist = animator.exist;
	this->ptr_attached_sprite = ptr_attached_sprite;

	for (const auto& animation : animator.animations)
	{
		// Copy animation
		spe::Animation anim = spe::Animation(this->ptr_attached_sprite, animation.second);
		animations.insert({ anim.GetName(), anim});
	}
}

void spe::Animator::init()
{
	this->base_component = false;
	this->exist = false;
	this->ptr_attached_sprite = nullptr;
	this->m_animation_playing.isAAnimationPlaying = false;
	this->m_animation_playing.name = "<Unknown>";
}

// Public functions

void spe::Animator::createAnimation(const std::string& name, const std::string& file_loc, const std::vector<spe::KeyFrame>& textures)
{
	EXIST_COMPONENT;
	animations.insert({ name, Animation(ptr_attached_sprite, name, file_loc, textures) });
}

void spe::Animator::removeAnimation(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->animations.find(name);
	animations.erase(name);
}

void spe::Animator::play(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		this->m_animation_playing.name = name;
		this->m_animation_playing.isAAnimationPlaying = true;
		it->second.Play();
	}
}

void spe::Animator::stop(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		this->m_animation_playing.name = "<Unknown>";
		this->m_animation_playing.isAAnimationPlaying = false;
		it->second.Stop();
	}
}

void spe::Animator::setName(const std::string& new_name, const std::string& old_name)
{
	EXIST_COMPONENT;

	auto it = animations.find(old_name);
	if (it != animations.end())
	{
		// The animation copy constructor handles the path renaming
		Animation animation = spe::Animation(this->ptr_attached_sprite, it->second, new_name);

		// Getting the old file to delete it
		animations.erase(it);

		animations[new_name] = animation;
	}
}

void spe::Animator::update()
{
	EXIST_COMPONENT;
	for (auto& anim : this->animations)
	{
		if (anim.second.IsPlaying)
		{
			anim.second.Update();
		}
	}
}

void spe::Animator::reset()
{
	this->m_animation_playing.name = "<Unknown>";
	this->m_animation_playing.isAAnimationPlaying = false;
	this->animations.clear();
}

//Static functions

//void spe::Animator::stopAllAnimations(spe::SpriteRepository& toUpdate)
//{
//	for (int i = 0; i < toUpdate.amount(); i++)
//	{
//		spe::Sprite* const sprite = toUpdate.readAt(i);
//		sprite->animator.stop(sprite->animator.m_animation_playing.name);
//	}
//}
