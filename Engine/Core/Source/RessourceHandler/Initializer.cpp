#include "initializer.h"
#include "Core/SeceneHandler.h"

// public static functions

#pragma region SPRITE

void spe::Initializer::InitSprites(spe::SpriteRepository& spriteRepo, const std::string& path, spe::LightRepository& lightrepo)
{
	//! INFO ! ALWAYS SCALE THINGS UP BY 1.5F!
	std::fstream spriteFile;
	//opening the file where all sprite data is
	spriteFile.open(path, std::ios::in);
	if (spriteFile.is_open())
	{
		spe::Sprite* current_sprite = nullptr;

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

			const std::vector<std::string> properties = spe::Utility::Split(line, PREFAB_DELIMITER);

			// S stands for sprite
			if (properties[0] == "S")
			{
				spe::Sprite* child = spe::Initializer::InitSprite(properties[1], lightrepo);
				current_sprite = child;
				spriteRepo.Add(child);
			}

			// A stands for animation
			if (properties[0] == "A")
			{
				spe::Initializer::InitAnimation(properties[1], current_sprite);
			}
		}
	}

	spriteFile.close();

	// Algorithm to set childs of the sprite
	std::list<spe::Sprite*>& sprites = spriteRepo.GetSprites();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;
		if (sprite->GetParentId() > 0)
		{
			if (!spriteRepo.ExistWithId(sprite->GetParentId()))
			{
				spe::Log::LogString("Sprite parent not found!");
				sprite->ClearParentData();
				continue;
			}
			spe::Sprite* parent = spriteRepo.GetById(sprite->GetParentId());
			if (parent != nullptr)
			{
				sprite->SetParent(parent);
			}
		}

	}
}


void spe::Initializer::InitAnimation(const std::string& path, spe::Sprite* spr)
{
	const std::string ext = "." + spe::Utility::GetFileExtension(path);
	if (ext != EXTENSION_ANIMATION_FILE)
	{
		throw std::runtime_error("Animation path file isnt ín the right format");
		return;
	}

	std::fstream animationFileStream;

	animationFileStream.open(path);

	spe::Sprite* ptr_sprite = spr;
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
				// Skipping the sprite get line
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
		spe::Log::LogString("Couldnt open animation file!");
	}

	if (ptr_sprite != nullptr)
	{
		ptr_sprite->Animator.CreateAnimation(animationName, path, frames);

		spe::Animation& anim = ptr_sprite->Animator.Animations[animationName];
		anim.Loop = loop;
	}
}

void spe::Initializer::InitAnimation(const std::string& path)
{
	const std::string ext =  "." + spe::Utility::GetFileExtension(path);
	if (ext != EXTENSION_ANIMATION_FILE)
	{
		throw std::runtime_error("Animation path file isnt ín the right format");
		return;
	}

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
		spe::Log::LogString("Couldnt open animation file!");
	}

	if (ptr_sprite != nullptr)
	{
		ptr_sprite->Animator.CreateAnimation(animationName, path, frames);

		spe::Animation& anim = ptr_sprite->Animator.Animations[animationName];
		anim.Loop = loop;
	}
}

spe::Sprite* spe::Initializer::InitSprite(const std::string& line, spe::LightRepository& lightrepo)
{
	std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

	const spe::Vector2 position = spe::Vector2(std::stof(propertys[2].c_str()), std::stof(propertys[3].c_str()));

	spe::Sprite* sprite = new spe::Sprite(propertys[0], position, propertys[6], lightrepo);

	spe::Vector2 vec(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str()));
	sprite->Transform.SetScale(vec, true);

	sprite->Transform.SetRotation(atoi(propertys[7].c_str()));

# pragma region Collider
	sprite->Collider.Width.X = std::stof(propertys[8].c_str());
	sprite->Collider.Width.Y = std::stof(propertys[9].c_str());

	sprite->Collider.Height.X = std::stof(propertys[10].c_str());
	sprite->Collider.Height.Y = std::stof(propertys[11].c_str());
	sprite->Collider.Exist = propertys[12] == "True";
	sprite->Collider.IsSolid = propertys[13] == "True";

#pragma endregion
# pragma region Sorting Layer
	sprite->SpriteRenderer.SortinLayerIdx = atoi(propertys[14].c_str());


#pragma endregion
# pragma region PhysicsBody
	sprite->Physicsbody.Gravity = std::stof(propertys[15].c_str());
	sprite->Physicsbody.Mass = std::stof(propertys[16].c_str());
	sprite->Physicsbody.Exist = propertys[17] == "True";

#pragma endregion
# pragma region parentId, ID
	sprite->SetId(atoi(propertys[18].c_str()));
	sprite->SetParentId(atoi(propertys[19].c_str()));

#pragma endregion
# pragma region Last pos, next pos


#pragma endregion

# pragma region Position to parent x, and y
	sprite->Transform.PositionToParent.X = std::stof(propertys[26]);
	sprite->Transform.PositionToParent.Y = std::stof(propertys[27]);

	sprite->Animator.Exist = propertys[28] == "True";
#pragma endregion

# pragma region Prefab
	sprite->Prefab.Exist = propertys[29] == "True";
	sprite->Prefab.LoadInMemory = propertys[30] == "True";
	sprite->Prefab.PathToFile = propertys[31];
	sprite->Prefab.UpdateName();
	sprite->Prefab.UpdatePath();
#pragma endregion

#pragma region General
	sprite->Tag = propertys[32];
#pragma endregion


#pragma region Light
	sprite->Light.SetRadius(std::stof(propertys[34]));
	sprite->Light.SetIntensity(std::stof(propertys[35]));
	if (propertys[33] == "True")
	{
		sprite->Light.Enable();
	}

	sprite->SpriteRenderer.EffectedByLight = propertys[36] == "True";

	spe::Vector3 color;

	color.X = std::stof(propertys[37]);
	color.Y = std::stof(propertys[38]);
	color.Z = std::stof(propertys[39]);

	sprite->Light.SetColor(spe::Vector3::ToSFVector3(color));
#pragma endregion

#pragma region render
	// sprite->SpriteRenderer.Render = propertys[40] == "True";
#pragma endregion

	sprite->Transform.SetOrigin();

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
			camera.SetZoom(std::stof(propertys[2].c_str()));
			camera.CameraSpeed = std::stof(propertys[3].c_str());
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
			vec.X = std::stof(propertys[0].c_str());
			vec.Y = std::stof(propertys[1].c_str());
			vec.Z = std::stof(propertys[2].c_str());
		}
		backgroundFile.close();
	}
}

#pragma endregion

spe::Sprite* spe::Initializer::InitPrefab(const std::string& path, spe::LightRepository& repo)
{
	std::fstream stream = std::fstream(path);

	if (stream.is_open())
	{
		spe::Sprite* current_sprite = nullptr;

		std::vector<spe::Sprite*> mini_repo;

		std::string line = "";
		uint8_t cnt = 0;
		while (getline(stream, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}
			const std::vector<std::string> properties = spe::Utility::Split(line, PREFAB_DELIMITER);
			
			// S stands for sprite
			if (properties[0] == "S")
			{
				spe::Sprite* child = spe::Initializer::InitSprite(properties[1], repo);
				current_sprite = child;
				mini_repo.push_back(child);
			}

			// A stands for animation
			if (properties[0] == "A")
			{
				spe::Initializer::InitAnimation(properties[1], current_sprite);
			}
		}

		// Parent algorithm to set childs
		for (int i = 0; i < mini_repo.size(); i++)
		{
			spe::Sprite* const sprite = mini_repo[i];
			if (sprite->GetParentId() > 0)
			{
				spe::Sprite* parent = spe::SpriteRepository::GetWithId(mini_repo, sprite->GetParentId());
				if (parent != nullptr)
				{
					sprite->ptr_Parent = parent;
					parent->ptr_Childs.push_back(sprite);
				}
			}
		}

		spe::Sprite* node = mini_repo[0]->GetNode();
		return node;
	}
	
	throw std::runtime_error("Couldn't open prefab file :(");
	return nullptr;
}

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
