#include "initializer.h"
#include "Core/SeceneHandler.h"

// public static functions

#pragma region SPRITE

void spe::Initializer::InitSprites(spe::SpriteRepository& spriteRepo, const std::string& path)
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
			// 
			//INITIIALIZING PROPS
			spe::Sprite* sprite =  spe::Initializer::InitSprite(line);

			//Pushing the sprite
			spriteRepo.Add(sprite);
		}
	}

	spriteFile.close();

	// Algorithm to set childs of the sprite
	std::list<spe::Sprite*>& sprites = spriteRepo.GetSprites();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;
		if (sprite->getParentId() > 0)
		{
			spe::Sprite* parent = spriteRepo.GetById(sprite->getParentId());
			if (parent != nullptr)
			{
				sprite->setParent(parent);
			}
		}

	}
}

void spe::Initializer::InitAnimations(spe::SpriteRepository& repo, const std::string& path)
{
	std::fstream animations;

	animations.open(path);

	if (animations.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(animations, line))
		{
			cnt++;
			if (cnt == 1) continue;
			spe::Initializer::initAnimation(line, repo, -1);
		}

		animations.close();
		return;
	}
	throw std::runtime_error("Animation file was not found");

}

void spe::Initializer::initAnimation(const std::string& path, spe::SpriteRepository& repo, int32_t idx)
{
	const std::string ext =  "." + spe::Utility::GetFileExtension(path);
	if (ext != EXTENSION_ANIMATION_FILE)
	{
		throw std::runtime_error("Animation path file isnt ín the right format");
		return;
	}
	bool load_idx = (idx != -1);

	std::fstream animationFileStream;

	animationFileStream.open(path);

	spe::Sprite* ptr_sprite = nullptr;
	std::string animationName = "";

	std::vector<spe::KeyFrame> frames = std::vector<spe::KeyFrame>(0);
	bool loop = false;
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
				uint32_t to_load = idx;
				if (!load_idx)
				{
					to_load = std::stoi(line);
				}
				ptr_sprite = repo.GetById(to_load);
				continue;
			}
			if (cnt == 3)
			{
				loop = line == "True";
				continue;
			}
			std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

			frames.push_back(spe::KeyFrame(propertys[1], std::stof(propertys[0].c_str())));
		}
	}
	else
	{
		throw std::runtime_error("Couldnt open animation file!");
	}

	if (ptr_sprite != nullptr)
	{
		ptr_sprite->animator.createAnimation(animationName, path, frames);

		spe::Animation& anim = ptr_sprite->animator.animations[animationName];
		anim.Loop = loop;
	}
}

spe::Sprite* spe::Initializer::InitSprite(const std::string& line)
{
	std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

	const spe::Vector2 position = spe::Vector2(std::stof(propertys[2].c_str()), std::stof(propertys[3].c_str()));

	spe::Sprite* sprite = new spe::Sprite(propertys[0], position, propertys[6]);

	sprite->collider = spe::BoxCollider(sprite);
	sprite->animator = spe::Animator(sprite);
	sprite->transform = spe::Transform(sprite);
	sprite->physicsBody = spe::PhsysicsBody();
	sprite->prefab = spe::Prefab(sprite);
	sprite->light = spe::Light(sprite);
	sprite->parent = nullptr;

	spe::Vector2 vec(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str()));
	sprite->transform.setScale(vec, true);

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

	sprite->transform.setOrigin();

	return sprite;
}

#pragma endregion

#pragma region CAMERA

void spe::Initializer::InitCamera(spe::Camera& camera, const std::string& path)
{
	std::fstream cameraFile;

	//opening the file where all sprite data is
	cameraFile.open(path, std::ios::in);
	if (cameraFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(cameraFile, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}

			//Splitting line
			std::string delimiter = ";";
			std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

			//INITIIALIZING PROPS
			const spe::Vector2 position(std::stof(propertys[0].c_str()), std::stof(propertys[1].c_str()));
		
			camera.Position = position;
			camera.setZoom(std::stof(propertys[2].c_str()));
			camera.camera_speed = std::stof(propertys[3].c_str());
		}
		cameraFile.close();
	}
}

#pragma endregion

#pragma region BACKGROUND

void spe::Initializer::InitBackground(spe::Vector3& vec, const std::string& path)
{
	std::fstream backgroundFile;

	//opening the file where all sprite data is
	backgroundFile.open(path, std::ios::in);
	if (backgroundFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(backgroundFile, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}

			//Splitting line
			std::string delimiter = ";";
			std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

			//INITIIALIZING PROPS
			vec.x = std::stof(propertys[0].c_str());
			vec.y = std::stof(propertys[1].c_str());
			vec.z = std::stof(propertys[2].c_str());
		}
		backgroundFile.close();
	}
}

#pragma endregion

void spe::Initializer::InitTags(spe::SpriteRepository& repo, const std::string& path)
{
	std::fstream tag_file;

	tag_file.open(path, std::ios::in);
	if (tag_file.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(tag_file, line))
		{
			cnt++;
			if (cnt == 1)
			{
				continue;
			}
			repo.Tags.push_back(line);
		}
		tag_file.close();
		return;
	}
	throw std::runtime_error("No tags file");
}

void spe::Initializer::InitScenes(SceneHandler& handler, const std::string& path)
{
	std::fstream scene_file;

	scene_file.open(path, std::ios::in);
	if (scene_file.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(scene_file, line))
		{
			cnt++;
			if (cnt == 1)
			{
				continue;
			}
			const std::string new_path = "Engine\\Saves\\" + line;
			if (std::filesystem::exists(new_path))
			{
				handler.TotalScenes.push_back(line);
			}
		}

		scene_file.close();
		return;
	}
	throw std::runtime_error("No scene file");
}

void spe::Initializer::IntiHighestSpriteID(spe::SpriteRepository& repo, const std::string& path)
{
	std::fstream indexFile;
	int index = 0;

	//opening the file where all sprite data is
	indexFile.open(path, std::ios::in);

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
	indexFile.close();

	repo.SetHighestId(index);
}

void spe::Initializer::PostInitAnimation(spe::Sprite* sprite, const std::string& file_path, spe::SpriteRepository& repo)
{
	spe::Initializer::initAnimation(file_path, repo, sprite->getId());
}