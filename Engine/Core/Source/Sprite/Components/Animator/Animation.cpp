#include "Animation.h"
#include "Sprite.h"

// Constructor / Destructor

spe::Animation::Animation()
{
	this->Loop = false;
	this->TotalTimePassed = 0.0f;
	this->TotalFramePassed = 0.0f;
	this->CurrentFrame = 0;
	this->IsPlaying = false;
	this->ptr_AppliedSprite = nullptr;
	this->TimePassed = 0.0f;
}

spe::Animation::Animation(Sprite* ptr_appliedSprite, const std::string& name, const std::string fileLocation, const std::vector<spe::KeyFrame>& frames)
{
	this->TotalFramePassed = 0.0f;
	this->m_BasePath = ptr_appliedSprite->sprite_renderer.path;
	this->m_PathToFile = fileLocation;
	this->TimePassed = 2.0f;
	this->CurrentFrame = -1;
	this->m_Name = name;
	this->ptr_AppliedSprite = ptr_appliedSprite;
	this->IsPlaying = false;
	this->TotalTimePassed = 0.0f;
	this->m_Keyframes.resize(frames.size());
	this->Loop = false;

	int CurrentPos = 0;
	for (int i = 0; i < frames.size(); i++)
	{
		this->m_Keyframes[i].path = frames[i].path;
		this->m_Keyframes[i].delay = frames[i].delay;
		CurrentPos += (int)this->m_Keyframes[i].delay;
		this->m_Keyframes[i].position = CurrentPos;
	}
	this->RealoadTextures();
}

spe::Animation::Animation(spe::Sprite* ptr_applied_sprite, const spe::Animation& animation)
{
	this->ptr_AppliedSprite = ptr_applied_sprite;
	this->InitCopyCtor(animation);
}

spe::Animation::Animation(spe::Sprite* ptr_applied_sprite, const spe::Animation& animation, const std::string& name)
{
	this->ptr_AppliedSprite = ptr_applied_sprite;
	this->InitCopyCtor(animation);
	this->m_Name = name;
}


void spe::Animation::InitCopyCtor(const spe::Animation& animation)
{
	this->ptr_AppliedSprite = ptr_AppliedSprite;
	this->m_Name = animation.m_Name;
	this->m_BasePath = this->ptr_AppliedSprite->sprite_renderer.path;
	this->Loop = animation.Loop;

	const std::vector<spe::KeyFrame>& keyframes = animation.GetkeyFrames();
	for (size_t i = 0; i < keyframes.size(); i++)
	{
		spe::KeyFrame copy = spe::KeyFrame(keyframes[i]);
		this->m_Keyframes.push_back(copy);
	}

	this->RealoadTextures();
}

// Public methods

void spe::Animation::DeleteKeyFrame(const int pos)
{
	float delay = 0;
	for (int i = 0; i < this->m_Keyframes.size(); i++)
	{
		delay += this->m_Keyframes[i].delay;

		if (delay == pos)
		{
			float delayAdd = this->m_Keyframes[i].delay;

			this->m_Keyframes.erase(this->m_Keyframes.begin() + i);
			this->m_Textures.erase(this->m_Textures.begin() + i);

			if (i + 1 > this->m_Keyframes.size())
			{
				return;
			}
			this->m_Keyframes[i].delay += delayAdd;

			return;
		}
	}
}

void spe::Animation::Play()
{
	this->m_BasePath = this->ptr_AppliedSprite->sprite_renderer.path;
	this->TimePassed = 0.0f;
	this->TotalFramePassed = 0;
	this->TotalTimePassed = 0.0f;
	this->CurrentFrame = 0;
	this->IsPlaying = true;
}

void spe::Animation::Update()
{
	this->TimePassed += Time::s_delta_time;
	this->TotalTimePassed += Time::s_delta_time;
	if (this->m_Keyframes.size() == 0)
	{
		return;
	}
	const float condition = this->m_Keyframes[CurrentFrame].delay / 100;
	if (this->TimePassed >= condition)
	{
		this->TotalFramePassed++;
		this->TimePassed = 0;
		this->ptr_AppliedSprite->setSpriteTexture(this->m_Textures[CurrentFrame], this->m_Keyframes[CurrentFrame].path);
		this->CurrentFrame++;
		if (this->CurrentFrame == this->m_Keyframes.size())
		{		
			if(this->Loop)
			{
				// Hard coding the path bc skill issue
				this->ptr_AppliedSprite->sprite_renderer.path = this->m_BasePath;
				this->Play();
			}
			else
			{
				this->Stop();
			}
		}
	}
}

void spe::Animation::Stop()
{
	this->TimePassed = 0.0f;
	this->TotalFramePassed = 0;
	this->TotalTimePassed = 0.0f;
	this->CurrentFrame = -1;
	this->IsPlaying = false;

	this->ptr_AppliedSprite->setSpriteTexture(this->m_BasePath);
}

spe::KeyFrame& spe::Animation::GetKeyFrameAtMs(const float ms)
{
	float delay = 0;
	for (int i = 0; i < this->m_Keyframes.size(); i++)
	{
		if (delay == ms)
		{
			return this->m_Keyframes[i];
		}

		delay += this->m_Keyframes[i].delay;
	}

	return this->m_Keyframes[0];
}


float spe::Animation::GetAnimationTime() const
{

	float time = 0.0f;
	for (size_t i = 0; i < this->m_Keyframes.size(); i++)
	{
		time += this->m_Keyframes[i].delay;
	}
	return time;
}

float spe::Animation::GetTimeTillFrame(size_t frame)
{
	if (frame == -1)
	{
		return -1;
	}
	float time = 0.0f;

	for (size_t i = 0; i < frame; i++)
	{
		time += this->m_Keyframes[i].delay;
	}	
	return time;
}

void spe::Animation::AddKeyFrameAt(const int vecpos, const spe::KeyFrame& frame)
{
	if (this->IsPlaying)
	{
		std::cout << "LOG: [WARNINING] Cant add keyframes while playing animation! " << frame.path << std::endl;
		return;
	}

	sf::Texture text;

	if (!text.loadFromFile(frame.path))
	{
		std::cout << "LOG: [ERROR] Cant read pixels from path " << frame.path << std::endl;
		return;
	}

	this->m_BasePath = this->ptr_AppliedSprite->sprite_renderer.path;
	this->m_Keyframes.insert(this->m_Keyframes.begin() + vecpos, frame);
	this->m_Textures.insert(this->m_Textures.begin() + vecpos, text);
}

void spe::Animation::RealoadTextures()
{
	this->m_Textures.clear();
	this->m_Textures = std::vector<sf::Texture>(this->m_Keyframes.size());

	for (int i = 0; i < this->m_Keyframes.size(); i++)
	{
		if (!this->m_Textures[i].loadFromFile(this->m_Keyframes[i].path))
		{
			std::cout << "LOG [ERROR] Couldnt load the animation file " + this->m_Keyframes[i].path + " !";
		}
	}
}

