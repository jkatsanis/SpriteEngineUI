#include "Animation.h"
#include <physicalComponents/sprite.h>

#include <engineComponents/input.h>

//Public functions

s2d::Animation::Animation(Sprite* ptr_appliedSprite, std::string name, std::vector<std::string> textures, float delay, bool useBaseSprite)
{
	this->timePassed = 2.0f;
	this->delay = delay;
	this->currentFrame = -1;
	this->name = name;
	this->ptr_appliedSprite = ptr_appliedSprite;
	this->isPlaying = false;
	this->m_useBaseSprite = useBaseSprite;
	this->m_usebaseNextFrame = (useBaseSprite) ? true : false;
	this->m_basePath = this->ptr_appliedSprite->path;
	this->m_paths = textures;

	this->setVectorSizes();
}

//Private methods

void s2d::Animation::setVectorSizes()
{
	if (this->m_useBaseSprite)
	{
		std::vector<std::string> temp = this->m_paths;
		this->m_paths.resize(this->m_paths.size() + 1);
		this->m_paths[0] = this->ptr_appliedSprite->path;

		for (int i = 1; i < this->m_paths.size(); i++)
		{
			this->m_paths[i] = temp[i - 1];
		}

		this->m_textures = std::vector<sf::Texture>(this->m_paths.size());

		for (int i = 0; i < this->m_paths.size(); i++)
		{
			this->m_textures[i].loadFromFile(this->m_paths[i]);
		}
	}
	else
	{

		this->m_textures = std::vector<sf::Texture>(this->m_paths.size());

		for (int i = 0; i < this->m_paths.size(); i++)
		{
			this->m_textures[i].loadFromFile(this->m_paths[i]);
		}
	}
}

//Public methods

//This method will start the animation and will set the current framt to zero
void s2d::Animation::play()
{
	this->currentFrame = 0;
	this->isPlaying = true;
}

//This methods updates the animation with the delay
void s2d::Animation::update()
{
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::B))
	{
		std::cout << this->m_textures.size() << std::endl;
		for (int i = 0; i < this->m_textures.size(); i++)
		{
			std::cout << this->m_paths[i] << std::endl;
		}
	}
	this->timePassed += Time::deltaTime;
	if (timePassed >= delay / 100)
	{
		this->timePassed = 0;
		std::cout << "y" << std::endl;
		this->ptr_appliedSprite->setSpriteTexture(this->m_textures[currentFrame]);
	    this->ptr_appliedSprite->transform.setScale(this->ptr_appliedSprite->transform.getScale(), true);
		this->ptr_appliedSprite->path = this->m_paths[currentFrame];

		if (this->currentFrame == this->m_textures.size() - 1)
		{
			this->currentFrame = -1;
			if (m_useBaseSprite)
			{
				m_usebaseNextFrame = true;
			}
		}

		this->currentFrame++;		
	}
}

void s2d::Animation::stop()
{
	this->currentFrame = -1;
	this->ptr_appliedSprite->setSpriteTexture(this->m_basePath);
	this->isPlaying = false;
}

void s2d::Animation::updateAllAnimations()
{
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		sprite->animator.update();
	}
}

