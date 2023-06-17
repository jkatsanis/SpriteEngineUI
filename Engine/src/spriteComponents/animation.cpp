#include "Animation.h"
#include <physicalComponents/sprite.h>

#include <engineComponents/input.h>

// Constructor / Destructor

s2d::Animation::Animation()
{
	this->total_time_passed = 0.0f;
	this->total_frame_passed = 0.0f;
	this->current_frame = 0;
	this->is_playing = false;
	this->m_saved_already = false;
	this->ptr_applied_sprite = nullptr;
	this->time_passed = 0.0f;
}

s2d::Animation::Animation(Sprite* ptr_appliedSprite, const std::string& name, const std::string fileLocation, const std::vector<s2d::KeyFrame>& frames)
{
	this->total_frame_passed = 0.0f;
	this->m_saved_already = false;
	this->m_base_path = ptr_appliedSprite->sprite_renderer.path;
	this->m_path_to_file = fileLocation;
	this->time_passed = 2.0f;
	this->current_frame = -1;
	this->name = name;
	this->ptr_applied_sprite = ptr_appliedSprite;
	this->is_playing = false;
	this->total_time_passed = 0.0f;
	this->m_keyframes.resize(frames.size());

	int currentPos = 0;
	for (int i = 0; i < frames.size(); i++)
	{
		this->m_keyframes[i].path = frames[i].path;
		this->m_keyframes[i].delay = frames[i].delay;
		currentPos += (int)this->m_keyframes[i].delay;
		this->m_keyframes[i].position = currentPos;
	}
	this->setVectorSizes();
}

// Public methods

void s2d::Animation::deleteKeyFrame(const int pos)
{
	int delay = 0;
	for (int i = 0; i < this->m_keyframes.size(); i++)
	{
		delay += this->m_keyframes[i].delay;

		if (delay == pos)
		{
			int delayAdd = this->m_keyframes[i].delay;

			std::removeAt(this->m_keyframes, i);
			std::removeAt(this->m_textures, i);

			if (i + 1 > this->m_keyframes.size())
			{
				return;
			}
			this->m_keyframes[i].delay += delayAdd;

			return;
		}
	}
}

void s2d::Animation::play()
{
	this->total_frame_passed = 0;
	this->total_time_passed = 0.0f;
	this->current_frame = 0;
	this->is_playing = true;
	this->m_base_path = this->ptr_applied_sprite->sprite_renderer.path;
}

void s2d::Animation::update()
{
	this->time_passed += Time::s_delta_time;
	this->total_time_passed += Time::s_delta_time;
	if (this->m_keyframes.size() == 0)
	{
		return;
	}
	if (this->time_passed >= this->m_keyframes[current_frame].delay / 100)
	{
		this->total_frame_passed++;
		this->time_passed = 0;
		this->ptr_applied_sprite->setSpriteTexture(this->m_textures[current_frame], this->m_keyframes[current_frame].path);
		this->current_frame++;
		if (this->current_frame == this->m_keyframes.size())
		{
			this->total_frame_passed = 0;
			this->total_time_passed = 0;
			this->current_frame = 0;
		}
	}
}

void s2d::Animation::stop()
{
	this->total_frame_passed = 0;
	this->total_time_passed = 0.0f;
	this->current_frame = -1;
	this->ptr_applied_sprite->setSpriteTexture(this->m_base_path);
	this->is_playing = false;
	this->m_base_path = "";
}

s2d::KeyFrame& s2d::Animation::getKeyFrameAtMs(const float ms)
{
	float delay = 0;
	for (int i = 0; i < this->m_keyframes.size(); i++)
	{
		if (delay == ms)
		{
			return this->m_keyframes[i];
		}

		delay += this->m_keyframes[i].delay;
	}

	return this->m_keyframes[0];
}

const std::string s2d::Animation::getEnginePathToFile() const
{
	return s2d::EngineData::s_path_to_user_project + "\\" +this->m_path_to_file;
}

float s2d::Animation::getAnimationTime() const
{
	float time = 0.0f;
	for (size_t i = 0; i < this->m_keyframes.size(); i++)
	{
		time += this->m_keyframes[i].delay;
	}
	return time;
}

float s2d::Animation::getTimeTillFrame(size_t frame)
{
	if (frame == -1)
	{
		return -1;
	}
	float time = 0.0f;

	for (size_t i = 0; i < frame; i++)
	{
		time += this->m_keyframes[i].delay;
	}	
	return time;
}

void s2d::Animation::addKeyFrameAt(const int vecpos, const s2d::KeyFrame& frame)
{
	if (this->is_playing)
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

	this->m_base_path = this->ptr_applied_sprite->sprite_renderer.path;
	this->m_keyframes.insert(this->m_keyframes.begin() + vecpos, frame);
	this->m_textures.insert(this->m_textures.begin() + vecpos, text);
}

void s2d::Animation::updateAllAnimations(s2d::SpriteRepository& repo)
{
	for (int i = 0; i < repo.amount(); i++)
	{
		s2d::Sprite* const sprite = repo.readAt(i);
		sprite->animator.update();
	}
}

//Private methods

void s2d::Animation::setVectorSizes()
{
	this->m_textures = std::vector<sf::Texture>(this->m_keyframes.size());

	for (int i = 0; i < this->m_keyframes.size(); i++)
	{
		this->m_textures[i].loadFromFile(this->m_keyframes[i].path);
	}
}
