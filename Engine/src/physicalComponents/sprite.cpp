 #include "sprite.h"

//Constructor / Destructor

s2d::Sprite::Sprite()
{
	this->name = "Unknown";
	this->transform.position = Vector2(0, 0);	
	this->m_texture = new sf::Texture();
}

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path)
{
	initVariables(name, spawnPosition, path);
}

s2d::Sprite::~Sprite()
{
	this->clearAllChilds();
	this->clearParentData();

	delete this->m_texture;
	this->m_texture = nullptr;
}

//Public functions

void s2d::Sprite::removeChild(const s2d::Sprite* child)
{
	if (child == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < this->ptr_childs.size(); i++)
	{
		if (child->getId() == this->ptr_childs[i]->getId())
		{
			this->ptr_childs.erase(this->ptr_childs.begin() + i);
			return;
		}
	}
}

void s2d::Sprite::clearAllChilds()
{
	this->ptr_childs.clear();
}

void s2d::Sprite::clearParentData()
{
	if (this->parent != nullptr)
	{
		this->parent->removeChild(this);
		this->parent = nullptr;
	}
	this->m_parent_id = 0;
}

void s2d::Sprite::setSpriteTexture(const std::string& path)
{
	if (!this->m_texture->loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->setSpriteTexture(*this->m_texture, path);
}

void s2d::Sprite::setSpriteTexture(const sf::Texture& texture, const std::string& path)
{
	this->m_sprite.setTexture(texture, true);
	this->transform.setScale(this->transform.getScale(), true);

	this->sprite_renderer.path = path;
}

void s2d::Sprite::setSpriteTexture(const std::string& path, const s2d::Vector2& scale)
{
	if (!this->m_texture->loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->m_sprite.setTexture(*this->m_texture);
	this->sprite_renderer.path = path;
	this->transform.setScale(scale, true);
}

void s2d::Sprite::setParent(s2d::Sprite* parent)
{
	if (parent == nullptr)
	{
		return;
	}

	// Clean up (Before) parent
	if (this->parent != nullptr)
	{
		this->parent->removeChild(this);
		this->parent = nullptr;
	}
	this->m_parent_id = parent->getId();
	this->parent = parent;

	s2d::Sprite* child = this;
	s2d::Vector2 distance = s2d::Vector2(parent->transform.position - child->transform.position);
	child->transform.position_to_parent = distance;

	parent->ptr_childs.push_back(this);
}

bool s2d::Sprite::containsChild(const s2d::Sprite* child) const
{
	bool contains = false;
	
	for (int i = 0; i < this->ptr_childs.size(); i++)
	{
		const s2d::Sprite* spr = this->ptr_childs[i];
		if (child->m_id == spr->getId())
		{
			return true;
		}
		contains = spr->containsChild(child);
	}
	return contains;
}

bool s2d::Sprite::containsChild(const ImGuiTextFilter& name) const
{
	bool contains = false;

	for (int i = 0; i < this->ptr_childs.size(); i++)
	{
		const s2d::Sprite* spr = this->ptr_childs[i];
		if (name.PassFilter(spr->name.c_str()))
		{
			return true;
		}
		contains = spr->containsChild(name);
	}
	return contains;
}

s2d::Vector2 s2d::Sprite::getOrigininalPosition() const
{
	float x = this->m_sprite.getPosition().x;
	float y = this->m_sprite.getPosition().y;

	return s2d::Vector2(x, y);
}

void s2d::Sprite::validateProperties(uint32_t id, s2d::SpriteRepository& repo)
{
	this->m_id = id;
	const char CHAR_INVALID_SYMBOLS[INVALID_SPRITE_SYMBOLS] = { ';' };

	std::string original_name = name;

	for (int i = 0; i < INVALID_SPRITE_SYMBOLS; i++)
	{
		name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
		if (name != original_name)
		{
			std::cout << "LOG [ERROR] Cant have invalid symbol!";
		}
	}
}


//Private functions

void s2d::Sprite::initVariables(std::string& name, s2d::Vector2& spawnPos, std::string& path)
{
	// ID's get managed by the sprite repo!

	this->m_texture = new sf::Texture();
	this->transform = s2d::Transform(this);
	this->m_parent_id = 0;
	this->parent = nullptr;
	this->ptr_childs = std::vector<s2d::Sprite*>(0);

	this->name = name;
	this->sprite_renderer.path = path;
	this->transform.position = spawnPos;	
	this->transform.lastPos = s2d::Vector2(0, 0);
	this->transform.nextPos = this->transform.position;
	this->sprite_renderer.sorting_layer_index = 0;
	this->animator = s2d::Animator(this);

	sf::Sprite sprite;

	if (!this->m_texture->loadFromFile(path))
	{
		std::cout << "LOG [ERROR] Couldnt load texture from file!";
	}

	sprite.setTexture(*this->m_texture);

	//Finally setting the sprite
	this->m_sprite = sprite;
	this->collider = s2d::BoxCollider(this);
	this->physicsBody = s2d::PhsysicsBody();
	this->prefab = s2d::Prefab(this);

	//Setting sprite size also in init and setTexture
	this->transform.setScale(s2d::Vector2(1, 1));
}
