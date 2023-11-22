#include "initializer.h"

// public static functions

void spe::Initializer::initSprites(spe::SpriteRepository& spriteRepo, const std::string& path)
{
	//! INFO ! ALWAYS SCALE THINGS UP BY 1.5F!
	std::fstream spriteFile;
	//opening the file where all sprite data is
	spriteFile.open(path, std::ios::in);
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

			//Creating empty sprite, then pushing it back
			Sprite* sprite = new Sprite();

			//INITIIALIZING PROPS
			spe::Initializer::initSprite(line, sprite);

			sprite->postInit();

			//Pushing the sprite
			spriteRepo.add(sprite);
		}
	}

	spriteFile.close();

	// Algorithm to set childs of the sprite
	std::list<spe::Sprite*>& sprites = spriteRepo.getSprites();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;
		if (sprite->getParentId() > 0)
		{
			spe::Sprite* parent = spriteRepo.getWithId(sprite->getParentId());
			if (parent != nullptr)
			{
				sprite->setParent(parent);
			}
		}

	}
}

// private static functions

void spe::Initializer::initSprite(const std::string& line, spe::Sprite* sprite)
{
	//Splitting line
	std::string delimiter = ";";
	std::vector<std::string> propertys = spe::Utility::Split(line, DELTIMITER);

	sprite->collider = spe::BoxCollider(sprite);
	sprite->animator = spe::Animator(sprite);
	sprite->transform = spe::Transform(sprite);
	sprite->physicsBody = spe::PhsysicsBody();
	sprite->prefab = spe::Prefab(sprite);
	sprite->light = spe::Light(sprite);
	sprite->parent = nullptr;

	sprite->name = propertys[0];
	const spe::Vector2 position = spe::Vector2(std::stof(propertys[2].c_str()), std::stof(propertys[3].c_str()));
	sprite->transform.SetPosition(position);

	sprite->setSpriteTexture(propertys[6], spe::Vector2(spe::Vector2(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str()))));

	sprite->transform.setRotation(atoi(propertys[7].c_str()));

# pragma region Collider
	sprite->collider.box_collider_width.x = std::stof(propertys[8].c_str());
	sprite->collider.box_collider_width.y = std::stof(propertys[9].c_str());

	sprite->collider.box_collider_height.x = std::stof(propertys[10].c_str());
	sprite->collider.box_collider_height.y = std::stof(propertys[11].c_str());
	sprite->collider.exist = propertys[12] == "True";
	sprite->collider.is_solid = propertys[13] == "True";

#pragma endregion
# pragma region Sorting Layer
	sprite->sprite_renderer.sorting_layer_index = atoi(propertys[14].c_str());


#pragma endregion
# pragma region PhysicsBody
	sprite->physicsBody.gravity = std::stof(propertys[15].c_str());
	sprite->physicsBody.mass = std::stof(propertys[16].c_str());
	sprite->physicsBody.exist = propertys[17] == "True";

#pragma endregion
# pragma region parentId, ID
	sprite->setId(atoi(propertys[18].c_str()));
	sprite->setParentId(atoi(propertys[19].c_str()));

#pragma endregion
# pragma region Last pos, next pos


#pragma endregion

# pragma region Position to parent x, and y
	sprite->transform.position_to_parent.x = std::stof(propertys[26]);
	sprite->transform.position_to_parent.y = std::stof(propertys[27]);

	sprite->animator.exist = propertys[28] == "True";
#pragma endregion

# pragma region Prefab
	sprite->prefab.exist = propertys[29] == "True";
	sprite->prefab.load_in_memory = propertys[30] == "True";
#pragma endregion

#pragma region General
	sprite->tag = propertys[32];
#pragma endregion


#pragma region Light
	sprite->light.setRadius(std::stof(propertys[34]));
	sprite->light.setIntensity(std::stof(propertys[35]));
	if (propertys[33] == "True")
	{
		sprite->light.enable();
	}

	sprite->sprite_renderer.effected_by_light = propertys[36] == "True";

	spe::Vector3 color;

	color.x = std::stof(propertys[37]);
	color.y = std::stof(propertys[38]);
	color.z = std::stof(propertys[39]);

	sprite->light.setColor(spe::Vector3::toSFVector3(color));
#pragma endregion
}