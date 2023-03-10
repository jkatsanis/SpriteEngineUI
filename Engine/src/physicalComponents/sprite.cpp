 #include "sprite.h"

//Constructor

s2d::Sprite::Sprite() { this->name = "Unknown"; this->transform.position = Vector2(0, 0); }

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path)
	: name(name)
{
	initVariables(name, spawnPosition, path);
}

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path, bool addToWindowByConstruction)
{
	initVariables(name, spawnPosition, path);

	if (addToWindowByConstruction)
		this->addSpriteToScene();
}

//Public functions

void s2d::Sprite::resetChildData()
{
	this->parent->m_childCount -= 1;
	this->parent->childs.erase(parent->childs.begin() + this->m_childListPos - 1);
	this->m_parentId = -1;
	this->parent = nullptr;
	this->m_childListPos = -1;
}

void s2d::Sprite::setSpriteTexture(const std::string& path)
{
	if (!this->m_texture.loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->setSpriteTexture(this->m_texture, path);
}

void s2d::Sprite::setSpriteTexture(const sf::Texture& texture, const std::string& path)
{
	this->m_sprite.setTexture(texture, true);
	this->transform.setScale(this->transform.getScale(), true);

	this->path = path;
}

void s2d::Sprite::setSpriteTexture(const std::string& path, const s2d::Vector2& scale)
{
	if (!this->m_texture.loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->m_sprite.setTexture(this->m_texture);
	this->path = path;
	this->transform.setScale(scale, true);
}

void s2d::Sprite::setParent(s2d::Sprite* parent)
{
	parent->m_childCount++;
	this->m_childListPos = parent->m_childCount + 1;
	this->parent = parent;
	this->m_parentId = parent->m_id;

	s2d::Transform::onPositionChange(this);

	parent->childs.push_back(this);
}

void s2d::Sprite::addSpriteToScene()
{
	this->m_vectorPosition = int(s2d::Sprite::s_sprites.size() + 1);
	s2d::Sprite::s_sprites.push_back(this);
}

bool s2d::Sprite::containsChild(const s2d::Sprite* child) const
{
	bool contains = false;
	for (s2d::Sprite* c : this->childs)
	{
		if (c->m_id == child->getId())
		{
			return true;
		}
		contains = c->containsChild(child);
	}
	return contains;
}

s2d::Vector2 s2d::Sprite::getOrigininalPosition() const
{
	float x = this->m_sprite.getPosition().x;
	float y = this->m_sprite.getPosition().y;

	return s2d::Vector2(x, y);
}


//Private functions

void s2d::Sprite::initVariables(std::string name, s2d::Vector2 spawnPos, std::string path)
{
	this->transform = s2d::Transform(this);

	this->m_childCount = -1;
	this->m_childListPos = -1;
	this->m_parentId = -1;

	// ID
	s2d::SpriteData::highestSpriteID++;
	this->setId(s2d::SpriteData::highestSpriteID);

	this->parent = nullptr;
	this->childs = std::vector<s2d::Sprite*>(0);

	this->name = name;
	this->path = path;
	this->transform.position = spawnPos;	
	this->transform.lastPos = s2d::Vector2(0, 0);
	this->transform.nextPos = this->transform.position;
	this->m_vectorPosition = -1;
	this->sortingLayerIndex = 0;
	this->animator = s2d::Animator(this);

	sf::Sprite sprite;

	if (!this->m_texture.loadFromFile(path))
	{
		//Console log!
	}

	sprite.setTexture(this->m_texture);

	//Finally setting the sprite
	this->m_sprite = sprite;
	this->collider = s2d::BoxCollider(this);
	this->physicsBody = s2d::PhsysicsBody();

	//Setting sprite size also in init and setTexture
	this->transform.setScale(s2d::Vector2(1, 1));
}

//Static functions

int s2d::Sprite::getMaxNumber(std::vector<s2d::Sprite*>& vec)
{
	if (vec.size() == 0)
	{
		return 0;
	}
	int max = vec[0]->getVectorPosition();
	for (int i = 1; i < vec.size(); i++)
	{
		if (max < vec[i]->getVectorPosition())
		{
			max = vec[i]->getVectorPosition();
		}
	}
	return max;
}

void s2d::Sprite::updateHightestLayerIndex()
{
	for (const s2d::Sprite* sprite : s2d::Sprite::s_sprites)
	{
		if (sprite->sortingLayerIndex > highestLayerIndex)
			highestLayerIndex = sprite->sortingLayerIndex;
	}
}

s2d::Sprite* s2d::Sprite::getSpriteById(int id)
{
	for (s2d::Sprite* sprite : s2d::Sprite::s_sprites)
	{
		if (sprite->m_id == id)
		{
			return sprite;
		}
	}
	return nullptr;
}

std::vector<s2d::Sprite*> s2d::Sprite::s_sprites(0);
int s2d::Sprite::highestLayerIndex = 0;