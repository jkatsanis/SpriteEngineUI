#include "initializer.h"

// public static functions

void s2d::Initializer::initAnimations(s2d::SpriteRepository& repo)
{
	//std::vector<s2d::AN
	std::fstream knownAnimationFileStream;

	knownAnimationFileStream.open(PATH_TO_KNOWN_ANIMATIONS);

	if (knownAnimationFileStream.is_open())
	{ 
		std::string line;
		int cnt = 0;
		while (std::getline(knownAnimationFileStream, line))
		{
			cnt++;
			if (cnt == 1) continue;
			s2d::Initializer::initAnimation(line, repo);
		}

		knownAnimationFileStream.close();
	}
	else std::cout << "LOG: [ERROR] could not open animation(s) file!";
}

void s2d::Initializer::initSprites(s2d::SpriteRepository& spriteRepo)
{
	//! INFO ! ALWAYS SCALE THINGS UP BY 1.5F!
	std::fstream spriteFile;


	//opening the file where all sprite data is
	spriteFile.open(PATH_TO_SPRITES, std::ios::in);
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
			std::vector<std::string> propertys = std::splitString(line, delimiter);

			//Creating empty sprite, then pushing it back
			Sprite* sprite = new Sprite();

			//INITIIALIZING PROPS

			sprite->collider = s2d::BoxCollider(sprite);
			sprite->animator = s2d::Animator(sprite);
			sprite->transform = s2d::Transform(sprite);

			sprite->name = propertys[0];
			sprite->transform.position.x = std::stof(propertys[2].c_str());
			sprite->transform.position.y = std::stof(propertys[3].c_str());

			sprite->setSpriteTexture(propertys[6], s2d::Vector2(s2d::Vector2(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str()) )));

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
			sprite->physicsBody.exists = propertys[17] == "True";

#pragma endregion
# pragma region parentId, ID
			sprite->setId(atoi(propertys[18].c_str()));
			sprite->setParentId(atoi(propertys[19].c_str()));

#pragma endregion
# pragma region Last pos, next pos
			sprite->transform.next_pos.x = std::stof(propertys[20]);
			sprite->transform.next_pos.y = std::stof(propertys[21]);

			sprite->transform.last_pos.x = std::stof(propertys[22]);
			sprite->transform.last_pos.y = std::stof(propertys[23]);

#pragma endregion

# pragma region Position to parent x, and y
			sprite->transform.position_to_parent.x = std::stof(propertys[26]);
			sprite->transform.position_to_parent.y = std::stof(propertys[27]);

			sprite->animator.exists = propertys[28] == "True";
#pragma endregion

			sprite->postInit();

			//Pushing the sprite
			spriteRepo.add(sprite);
		}
	}

	spriteFile.close();

	//setting childs of sprites
	for (int i = 0; i < spriteRepo.amount(); i++)
	{
		s2d::Sprite* const sprite = spriteRepo.readAt(i);
		if (sprite->getParentId() > 0)
		{
			s2d::Sprite* parent = spriteRepo.getSpriteWithId(sprite->getParentId());
			if (parent != nullptr)
			{
				sprite->parent = parent;
				parent->ptr_childs.push_back(sprite);
			}
		}
	}

}

void s2d::Initializer::initIds(uint32_t& highestId)
{
	std::fstream indexFile;
	int index = 0;

	//opening the file where all sprite data is
	indexFile.open("engine\\saves\\index.txt", std::ios::in);
	if (indexFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(indexFile, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}

			index = atoi(line.c_str());
		}
	}
	highestId = index;
}

// private static functions

void s2d::Initializer::initAnimation(const std::string& path, s2d::SpriteRepository& spriteRepo)
{
	std::fstream animationFileStream;

	animationFileStream.open(path);

	s2d::Sprite* ptr_sprite = nullptr;
	std::string animationName = "";

	std::vector<s2d::KeyFrame> frames = std::vector<s2d::KeyFrame>(0);

	if (animationFileStream.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(animationFileStream, line))
		{
			cnt++;
			if (cnt == 1)
			{
				animationName = line;
				continue;
			}
			if (cnt == 2)
			{
				// read only the ID of the sprite to apply the animtion
				int idx = std::stoi(line);
				ptr_sprite = spriteRepo.getSpriteWithId(idx);
				continue;
			}
			std::vector<std::string> propertys = std::splitString(line, DELIMITER);

			if(propertys[1] != "")
				frames.push_back(s2d::KeyFrame(propertys[1], atoi(propertys[0].c_str())));
		}

	}
	else std::cout << "LOG: [ERROR] could not open animation data file!";
	if (ptr_sprite != nullptr)
	{
		ptr_sprite->animator.createAnimation(animationName, path, frames);
	}
}
