#include "Animation.h"
#include <physicalComponents/sprite.h>

#include <engineComponents/input.h>

// Constructor

s2d::Animation::Animation(Sprite* ptr_appliedSprite, const std::string& name, const std::string fileLocation, const std::vector<s2d::KeyFrame>& frames)
{
	this->m_basePath = ptr_appliedSprite->path;
	this->m_pathToFile = fileLocation;
	this->timePassed = 2.0f;
	this->currentFrame = -1;
	this->name = name;
	this->ptr_appliedSprite = ptr_appliedSprite;
	this->isPlaying = false;
	
	this->m_keyFrames.resize(frames.size());

	int currentPos = 0;
	for (int i = 0; i < frames.size(); i++)
	{
		this->m_keyFrames[i].path = frames[i].path;
		this->m_keyFrames[i].delay = frames[i].delay;
		currentPos += (int)this->m_keyFrames[i].delay;
		this->m_keyFrames[i].position = currentPos;
	}
	this->setVectorSizes();
}

//Private methods

void s2d::Animation::setVectorSizes()
{
	this->m_textures = std::vector<sf::Texture>(this->m_keyFrames.size());

	for (int i = 0; i < this->m_keyFrames.size(); i++)
	{
		this->m_textures[i].loadFromFile(this->m_keyFrames[i].path);
	}
}

//Public methods

void s2d::Animation::deleteKeyFrame(const int pos)
{
	int delay = 0;
	for (int i = 0; i < this->m_keyFrames.size(); i++)
	{
		delay += this->m_keyFrames[i].delay;

		if (delay == pos)
		{
			int delayAdd = this->m_keyFrames[i].delay;

			std::removeAt(this->m_keyFrames, i);
			std::removeAt(this->m_textures, i);

			if (i + 1 > this->m_keyFrames.size())
			{
				return;
			}
			this->m_keyFrames[i].delay += delayAdd;

			return;
		}
	}
}

void s2d::Animation::play()
{
	this->currentFrame = 0;
	this->isPlaying = true;
	this->m_basePath = this->ptr_appliedSprite->path;
}

void s2d::Animation::update()
{
	this->timePassed += Time::deltaTime;
	if (this->m_keyFrames.size() == 0)
	{
		return;
	}
	if (this->timePassed >= this->m_keyFrames[currentFrame].delay / 100)
	{
		this->timePassed = 0;
		this->ptr_appliedSprite->setSpriteTexture(this->m_textures[currentFrame], this->m_keyFrames[currentFrame].path);
		this->currentFrame++;		
		if (this->currentFrame == this->m_keyFrames.size())
		{
			this->currentFrame = 0;
		}
	}
}

void s2d::Animation::stop()
{
	this->currentFrame = -1;
	this->ptr_appliedSprite->setSpriteTexture(this->m_basePath);
	this->isPlaying = false;
	this->m_basePath = "";
}

s2d::KeyFrame& s2d::Animation::getKeyFrameAtMs(const float ms)
{
	float delay = 0;
	for (int i = 0; i < this->m_keyFrames.size(); i++)
	{
		if (delay == ms)
		{
			return this->m_keyFrames[i];
		}

		delay += this->m_keyFrames[i].delay;
	}

	return this->m_keyFrames[0];
}

void s2d::Animation::addKeyFrameAt(const int vecpos, const s2d::KeyFrame& frame)
{
	if (this->isPlaying)
	{
		std::cout << "LOG: [Warning] Cant add keyframes while playing animation! " << frame.path << std::endl;
		return;
	}

	sf::Texture text;

	if (!text.loadFromFile(frame.path))
	{
		std::cout << "LOG: [ERROR] Cant read pixels from path " << frame.path << std::endl;
		return;
	}

	this->m_basePath = this->ptr_appliedSprite->path;
	this->m_keyFrames.insert(this->m_keyFrames.begin() + vecpos, frame);
	this->m_textures.insert(this->m_textures.begin() + vecpos, text);
}

void s2d::Animation::updateAllAnimations()
{
	for (s2d::Sprite* sprite : s2d::Sprite::s_sprites)
	{
		sprite->animator.update();
	}
}

