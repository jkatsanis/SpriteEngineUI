#include "Animator.h"
#include <physicalComponents/sprite.h>
#include <manager/saveSystem.h>

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
		animations.insert({ anim.getName(), anim});
	}
}

void s2d::Animator::init()
{
	this->base_component = false;
	this->exist = false;
	this->ptr_attached_sprite = nullptr;
	this->m_animation_playing.isAAnimationPlaying = false;
	this->m_animation_playing.name = "<Unknown>";
}

// Public functions

void s2d::Animator::createAnimation(const std::string& name, const std::string& file_loc, const std::vector<s2d::KeyFrame>& textures)
{
	EXIST_COMPONENT;
	animations.insert({ name, Animation(ptr_attached_sprite, name, file_loc, textures) });
}

void s2d::Animator::removeAnimation(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		std::removeFile(it->second.getEnginePathToFile());
	}
	animations.erase(name);
}

void s2d::Animator::play(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		this->m_animation_playing.name = name;
		this->m_animation_playing.isAAnimationPlaying = true;
		it->second.play();
	}
}

void s2d::Animator::stop(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		this->m_animation_playing.name = "<Unknown>";
		this->m_animation_playing.isAAnimationPlaying = false;
		it->second.stop();
	}
}

void s2d::Animator::setName(const std::string& new_name, const std::string& old_name)
{
	EXIST_COMPONENT;

	auto it = animations.find(old_name);
	if (it != animations.end())
	{
		// The animation copy constructor handles the path renaming
		Animation animation = s2d::Animation(this->ptr_attached_sprite, it->second, new_name);

		// Getting the old file to delete it
		const std::string old_path = s2d::EngineData::s_path_to_user_project + "\\"+ it->second.getUserPathToFile();
		animations.erase(it);

		animations[new_name] = animation;

		// Deleting old file, creating new file
		std::removeFile(old_path);
		s2d::flc::createAnimationSaveFile(this->ptr_attached_sprite, animation);

	}

}

void s2d::Animator::update()
{
	EXIST_COMPONENT;
	for (auto& anim : this->animations)
	{
		if (anim.second.is_playing)
		{
			anim.second.update();
		}
	}
}

void s2d::Animator::reset()
{
	this->m_animation_playing.name = "<Unknown>";
	this->m_animation_playing.isAAnimationPlaying = false;
	this->animations.clear();
}

void s2d::Animator::flagAllAnimationsToNotDelete()
{
	for (auto& pair : this->animations)
	{
	    auto& value = pair.second;
		value.setFlagToNotDeleteAfterExit();
	}
}

//Static functions

void s2d::Animator::stopAllAnimations(s2d::SpriteRepository& toUpdate)
{
	for (int i = 0; i < toUpdate.amount(); i++)
	{
		s2d::Sprite* const sprite = toUpdate.readAt(i);
		sprite->animator.stop(sprite->animator.m_animation_playing.name);
	}
}
