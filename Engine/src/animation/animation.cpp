#include "Animation.h"
#include "Sprite.h"

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

	setVectorSizes(textures);
}

//Private methods

void s2d::Animation::setVectorSizes(const std::vector<std::string>& textures)
{
	if (this->m_useBaseSprite)
	{
		this->m_paths = std::vector<std::string>(textures.size() + 1);

		for (int i = 1; i <= textures.size(); i++)
		{
			this->m_paths[i] = textures[i - 1];
		}

		this->m_textures = std::vector<sf::Texture>(textures.size() + 1);
		this->m_textures[0].loadFromFile(this->ptr_appliedSprite->path);
		for (int i = 1; i <= textures.size(); i++)
		{
			this->m_textures[i].loadFromFile(textures[i - 1]);
		}
	}
	else
	{

		this->m_textures = std::vector<sf::Texture>(textures.size());

		for (int i = 0; i < textures.size(); i++)
		{
			this->m_textures[i].loadFromFile(textures[i]);
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
	this->timePassed += Time::deltaTime;
	if (timePassed >= delay / 100)
	{
		this->timePassed = 0;

		this->ptr_appliedSprite->getSprite().setTexture(this->m_textures[currentFrame], true);
		sf::Vector2u tempSize = this->m_textures[currentFrame].getSize();
		this->ptr_appliedSprite->transform.scale = Vector2(tempSize.x, tempSize.y);
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

