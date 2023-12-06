#include "Animator.h"

#include "Sprite.h"

// Constructor / Destructor

spe::Animator::Animator()
{
	this->Init();
}

spe::Animator::Animator(Sprite* ptr_attached_sprite)
{
	this->Init();
	this->ptr_attached_sprite = ptr_attached_sprite;
}

spe::Animator::Animator(spe::Sprite* ptr_attached_sprite, spe::Animator& animator)
{
	this->Init();
	this->Exist = animator.Exist;
	this->ptr_attached_sprite = ptr_attached_sprite;

	for (const auto& animation : animator.Animations)
	{
		// Copy animation
		spe::Animation anim = spe::Animation(this->ptr_attached_sprite, animation.second);
		Animations.insert({ anim.GetName(), anim});
	}
}

void spe::Animator::Init()
{
	this->BaseComponent = false;
	this->Exist = false;
	this->ptr_attached_sprite = nullptr;
	this->m_AnimationPlaying.IsPlaying = false;
	this->m_AnimationPlaying.Name = "<Unknown>";
}

// Public functions

void spe::Animator::CreateAnimation(const std::string& name, const std::string& file_loc, const std::vector<spe::KeyFrame>& textures)
{
	EXIST_COMPONENT;
	Animations.insert({ name, Animation(ptr_attached_sprite, name, file_loc, textures) });
}

void spe::Animator::RemoveAnimation(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->Animations.find(name);
	Animations.erase(name);
}

void spe::Animator::Play(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->Animations.find(name);
	if (it != this->Animations.end())
	{
		this->m_AnimationPlaying.Name = name;
		this->m_AnimationPlaying.IsPlaying = true;
		it->second.Play();
	}
}

void spe::Animator::Stop(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->Animations.find(name);
	if (it != this->Animations.end())
	{
		this->m_AnimationPlaying.Name = "<Unknown>";
		this->m_AnimationPlaying.IsPlaying = false;
		it->second.Stop();
	}
}

void spe::Animator::SetName(const std::string& new_name, const std::string& old_name)
{
	EXIST_COMPONENT;

	auto it = Animations.find(old_name);
	if (it != Animations.end())
	{
		// The animation copy constructor handles the path renaming
		Animation animation = spe::Animation(this->ptr_attached_sprite, it->second, new_name);

		// Getting the old file to delete it
		Animations.erase(it);

		Animations[new_name] = animation;
	}
}

void spe::Animator::Update()
{
	EXIST_COMPONENT;
	for (auto& anim : this->Animations)
	{
		if (anim.second.IsPlaying)
		{
			anim.second.Update();
		}
	}
}

void spe::Animator::Reset()
{
	this->m_AnimationPlaying.Name = "<Unknown>";
	this->m_AnimationPlaying.IsPlaying = false;
	this->Animations.clear();
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
