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

	if(addToWindowByConstruction)
		s2d::Sprite::activeSprites.push_back(this);
}

//Public functions

void s2d::Sprite::resetChildData()
{
	this->parent->m_childCount -= 1;
	this->parent->childs.erase(parent->childs.begin() + this->getChildListPosition() - 1);
	this->m_parentId = -1;
	this->parent = nullptr;
	this->m_childListPos = -1;
}

void s2d::Sprite::setSpriteTexture(std::string path)
{
	if (!this->m_texture.loadFromFile(path))
	{
		//No file
	}
	this->path = path;
	this->m_sprite.setTexture(this->m_texture, true);

	//Setting sprite size also in init and setTexture
	sf::Vector2u tempSize = m_texture.getSize();
	this->transform.scale = s2d::Vector2(float(tempSize.x), float(tempSize.y));
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
	this->m_vectorPosition = int(s2d::Sprite::activeSprites.size() + 1);
	s2d::Sprite::activeSprites.push_back(this);
}

bool s2d::Sprite::containsChild(s2d::Sprite* child)
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

s2d::Vector2 s2d::Sprite::getOrigininalPosition()
{
	float x = this->m_sprite.getPosition().x;
	float y = this->m_sprite.getPosition().y;

	return s2d::Vector2(x, y);
}

void s2d::Sprite::update()
{
	this->transform.updateTransformPosition();
}

//Private functions

void s2d::Sprite::initVariables(std::string name, s2d::Vector2 spawnPos, std::string path)
{
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

	//Setting sprite size also in init and setTexture
	setTextureSize();

	//Finally setting the sprite
	this->m_sprite = sprite;

	this->transform = s2d::Transform(this);
	this->collider = s2d::BoxCollider(this);
	this->physicsBody = s2d::PhsysicsBody();
}

void s2d::Sprite::setTextureSize()
{
	sf::Vector2u tempSize = m_texture.getSize();
	this->transform.scale = Vector2(float(tempSize.x), float(tempSize.y));
}

//Static functions

void s2d::Sprite::initActiveSprites()
{
	std::fstream spriteFile;

	//opening the file where all sprite data is

	spriteFile.open(PATH_TO_SPRITE_FILE, std::ios::in);
	if (spriteFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(spriteFile, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}

			//Splitting line
			std::string delimiter = ";";
			std::string* propertys = std::splitString(line, delimiter);

			//Creating empty sprite, then pushing it back
			Sprite* sprite = new Sprite();

			//INITIIALIZING PROPS
			
			// Components
			sprite->transform = s2d::Transform(sprite);
			sprite->collider = BoxCollider(sprite);
			sprite->animator = s2d::Animator(sprite);

			sprite->name = propertys[0];
			sprite->setVectorPosition(atoi(propertys[1].c_str()));
			sprite->transform.position.x = std::stof(propertys[2].c_str());
			sprite->transform.position.y = std::stof(propertys[3].c_str());
			sprite->path = s2d::EngineData::s_pathToUserProject + "\\" + propertys[4];

			//INFO: Setting box collider props 5 - 8 down lol

			if (!sprite->m_texture.loadFromFile(sprite->path))
			{
				//Console log!
			}

			//Setting sprite size also in init and setTexture
			sf::Vector2u tempSize = sprite->m_texture.getSize();
			sprite->transform.scale = Vector2(float(tempSize.x), float(tempSize.y));
			sprite->m_sprite.setTexture(sprite->m_texture);
	
			//Collider
			sprite->collider.boxColliderWidthLeftOrRight.x = std::stof(propertys[5].c_str());
			sprite->collider.boxColliderWidthLeftOrRight.y = std::stof(propertys[6].c_str());

			sprite->collider.boxColliderHeightUpOrDown.x = std::stof(propertys[7].c_str());
			sprite->collider.boxColliderHeightUpOrDown.y = std::stof(propertys[8].c_str());
			sprite->collider.exists = propertys[9] == "True";
			sprite->collider.isSolid = propertys[10] == "True";

			//Sorting Layer
			sprite->sortingLayerIndex = atoi(propertys[11].c_str());

			//PhysicsBody
			sprite->physicsBody.gravity = std::stof(propertys[12].c_str());
			sprite->physicsBody.mass = std::stof(propertys[13].c_str());
			sprite->physicsBody.exists = propertys[14] == "True";

			//parentId, ID
			sprite->m_id = atoi(propertys[15].c_str());
			sprite->m_parentId = atoi(propertys[16].c_str());

			//Last pos, next pos
			sprite->transform.nextPos.x = std::stof(propertys[17]);
			sprite->transform.nextPos.y = std::stof(propertys[18]);

			sprite->transform.lastPos.x = std::stof(propertys[19]);
			sprite->transform.lastPos.y = std::stof(propertys[20]);

			//list pos
			sprite->m_childListPos = atoi(propertys[21].c_str());
			sprite->m_childCount = atoi(propertys[22].c_str());

			//Position to parent x, and y
			sprite->transform.positionToParent.x = std::stof(propertys[23]);
			sprite->transform.positionToParent.y = std::stof(propertys[24]);

			sprite->animator.exists = propertys[25] == "True";

			//Pushing the sprite
			s2d::Sprite::activeSprites.push_back(sprite);

			//End of INITING

			//Deleting the propertys since we dont need them anymore
			delete[] propertys;
		}
	}

	spriteFile.close();

	//setting childs of sprites
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (sprite->m_parentId > 0)
		{
			s2d::Sprite* parent = s2d::Sprite::getSpriteById(sprite->m_parentId);
			if (parent != nullptr)
			{
				sprite->parent = parent;
				parent->childs.push_back(sprite);
			}
		}
	}
}

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
	for (const s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (sprite->sortingLayerIndex > highestLayerIndex)
			highestLayerIndex = sprite->sortingLayerIndex;
	}
}

s2d::Sprite* s2d::Sprite::getSpriteById(int id)
{
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (sprite->m_id == id)
		{
			return sprite;
		}
	}
	return nullptr;
}

std::vector<s2d::Sprite*> s2d::Sprite::activeSprites(0);
int s2d::Sprite::highestLayerIndex = 0;