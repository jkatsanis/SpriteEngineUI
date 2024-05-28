#include "Savesystem.h"

// Private


// Public 

void spe::Savesystem::SaveEverything(const spe::SpriteRepository& repo, const spe::Camera& gui,
	const spe::Vector3& bg, const spe::SceneHandler& scene)
{
	ALERT_IF_CANT_SAVE;

	spe::Savesystem::UpdateSceneFile(scene);
	spe::Savesystem::UpdateCameraFile(gui);
	spe::Savesystem::UpdateBackgroundFile(bg);
	spe::Savesystem::UpdateSpriteFile(repo);
	spe::Savesystem::UpdateTagsFile(repo);
	spe::Savesystem::UpdateHighestIndexFile(repo.GetHighestId());
	spe::Savesystem::UpdateAnimationFile(repo);
}


void spe::Savesystem::SaveProjects(const std::vector<spe::UserProjectInfo>& projects)
{
	std::string content;

	for (const spe::UserProjectInfo& project : projects)
	{
		content += project.Name + ";" + project.AbsulutePath + ";" + project.LastOpened + ";" + project.relativePath + "\n";
	}

	std::ofstream knownProjectFile;

	knownProjectFile.open(PATH_TO_KNOWN_PROJECTS, std::ios::out);
	if (knownProjectFile.is_open())
	{
		knownProjectFile << "name;absulutePathStr;date;relativePath" << "\n";

		knownProjectFile << content << "\n";

		knownProjectFile.close();
	}

	for (const spe::UserProjectInfo& project : projects)
	{
		const std::string path = project.AbsulutePath + "\\Engine\\Saves\\enginepath.txt";
		std::ofstream corePath;

		corePath.open(path);

		if (!corePath.is_open() && std::filesystem::exists(project.AbsulutePath))
		{
			// Removing the project from the list
			spe::Utility::Delete(project.AbsulutePath);
		}

		const std::string pathCore = spe::Utility::GetCurrentDir();
		
		corePath << pathCore << "\n";
	}

}

#pragma region SPRITE

std::string spe::Savesystem::GetPropertyLineWithSeperator(const spe::Sprite* sprite)
{
	std::string line;
	const std::string transformPosX = std::to_string(sprite->Transform.GetPosition().X);
	const std::string transformPosY = std::to_string(sprite->Transform.GetPosition().Y);
	const std::string scaleX = std::to_string(sprite->Transform.GetScale().X);
	const std::string scaleY = std::to_string(sprite->Transform.GetScale().Y);
	const std::string spritePath = sprite->SpriteRenderer.Path;

	const std::string boxColliderWidthLeftOrRightX = std::to_string(sprite->Collider.Width.X);
	const std::string boxColliderWidthLeftOrRightY = std::to_string(sprite->Collider.Width.Y);

	const std::string boxColliderHeightUpOrDownX = std::to_string(sprite->Collider.Height.X);
	const std::string boxColliderHeightUpOrDownY = std::to_string(sprite->Collider.Height.Y);

	const std::string colliderExists = spe::Utility::BoolToStr(sprite->Collider.Exist);
	const std::string isSolid = spe::Utility::BoolToStr(sprite->Collider.IsSolid);
	const std::string sortingLayer = std::to_string(sprite->SpriteRenderer.SortinLayerIdx);
	const std::string gravity = std::to_string(sprite->Physicsbody.Gravity);
	const std::string mass = std::to_string(sprite->Physicsbody.Mass);
	const std::string bodyExist = spe::Utility::BoolToStr(sprite->Physicsbody.Exist);
	const std::string id = std::to_string(sprite->GetId());
	const std::string parentId = std::to_string(sprite->GetParentId());

	const std::string positionToParentX = std::to_string(sprite->Transform.PositionToParent.X);
	const std::string positionToParentY = std::to_string(sprite->Transform.PositionToParent.Y);

	const std::string animatorExist = spe::Utility::BoolToStr(sprite->Animator.Exist);

	const std::string prefabExist = spe::Utility::BoolToStr(sprite->Prefab.Exist);
	const std::string loadInMemory = spe::Utility::BoolToStr(sprite->Prefab.LoadInMemory);
	const std::string pathToPrefab = sprite->Prefab.PathToFile;
	const std::string rotation = std::to_string(sprite->Transform.GetRotation());
	const std::string tag = sprite->Tag;

	const std::string lightExist = spe::Utility::BoolToStr(sprite->Light.Exist);
	const std::string light_radiues = std::to_string(sprite->Light.GetRadius());
	const std::string light_intensy = std::to_string(sprite->Light.GetIntensity());
	const std::string effected_by_light = spe::Utility::BoolToStr(sprite->SpriteRenderer.EffectedByLight);

	const std::string light_color_r = std::to_string(sprite->Light.GetColor().x);
	const std::string light_color_g = std::to_string(sprite->Light.GetColor().y);
	const std::string light_color_b = std::to_string(sprite->Light.GetColor().z);

	const std::string render = spe::Utility::BoolToStr(sprite->SpriteRenderer.Render);

	//Name, vec, transform path, rotation
	line = sprite->Name + ";" + "0" + ";" + transformPosX + ";" + transformPosY + ";" + scaleX + ";" + scaleY + ";" + spritePath + ";" + rotation;

	//BoxCollider
	line += ";" + boxColliderWidthLeftOrRightX + ";" + boxColliderWidthLeftOrRightY + ";" + boxColliderHeightUpOrDownX + ";" + boxColliderHeightUpOrDownY + ";" + colliderExists + ";" + isSolid;

	//Sorting layer
	line += ";" + sortingLayer;

	//PhysicsBody
	line += ";" + gravity + ";" + mass + ";" + bodyExist;

	//ParentId, id
	line += ";" + id + ";" + parentId;

	//next pos, last pos
	line += ";0;0;0;0";

	//list pos ( childing)
	line += ";" + std::string("listpos") + ";" + "childCount";

	//Pos to parent (x, y)
	line += ";" + positionToParentX + ";" + positionToParentY;

	//ANIMATIONS

	line += ";" + animatorExist;

	// Prefab

	line += ";" + prefabExist + ";" + loadInMemory + ";" + pathToPrefab;

	// General
	line += ";" + tag;

	// Light

	line += ";" + lightExist + ";" + light_radiues + ";" + light_intensy;

	line += ";" + effected_by_light;

	line += ";" + light_color_r + ";" + light_color_g + ";" + light_color_b;

	// Render
	line += ";" + render;

	return line;
}

#pragma endregion

void spe::Savesystem::CreateOrUpdatePrefabFile(const spe::Sprite* content, const std::string& pathToFile, const std::string& oldFilePath)
{
	//Getting filelocation as: \\assets

	if (pathToFile != oldFilePath && oldFilePath != "")
	{
		spe::Utility::Delete(oldFilePath);
	}

	std::string fileContent = "name;vecpos;transformPosX;transformPosY;ScaleX;ScaleY;filepath;boxColliderWidthLeftOrRightX;boxColliderWidthLeftOrRighY;boxColliderHeightUpOrDownX;boxColliderHeightUpOrDownY;boxColliderExists;solid;sortingLayer;gravity;mass;physicsBodyExists;id;parentId;nextPosX;nextPosY;lastPosX;lastPosY;listPos;highestChild;positionToParentX;positionToParentY;animatorExists;prefabExist;loadInMemory;pathToPrefab;render\n";

	std::string s;
	s.push_back(PREFAB_DELIMITER);

	fileContent += "S" + s + spe::Savesystem::GetPropertyLineWithSeperator(content) + "\n";


	// Animation of parent
	for (auto& animation : content->Animator.Animations)
	{
		const auto& value = animation.second;
		fileContent += "A" + s + value.GetPath() + "\n";
	}

	std::vector<const spe::Sprite*> childs;

	spe::SpriteRepository::GetAllChilds(childs, content);

	// Initing childs
	for (size_t i = 0; i < childs.size(); i++)
	{
		fileContent += "S" + s + spe::Savesystem::GetPropertyLineWithSeperator(childs[i]) + "\n";
		for (auto& animation : childs[i]->Animator.Animations)
		{
			const auto& value = animation.second;
			fileContent += "A" + s + value.GetPath() + "\n";
		}
	}
	
	spe::Utility::CreateFileWithContent(fileContent, pathToFile);
}


void spe::Savesystem::CreateAnimationSaveFile(const spe::Sprite* ptr_sprite, const spe::Animation& anim)
{
	std::string name = anim.GetName();
	std::string content =
		anim.GetName() + "\n" +
		std::to_string(ptr_sprite->GetId()) + "\n" +
		spe::Utility::BoolToStr(anim.Loop) + "\n";

	const std::vector<spe::KeyFrame>& frames = anim.GetkeyFrames();

	for (const spe::KeyFrame& frame : frames)
	{
		content += std::to_string(frame.delay) + std::string(";") +
			(frame.path) + "\n";
	}
	std::string pathAndName = anim.GetPath();
	spe::Utility::CreateFileWithContent(content, pathAndName);
}

void spe::Savesystem::UpdateSceneFile(const spe::SceneHandler& handler)
{
	std::string content = "Scenes\n";

	for (size_t i = 0; i < handler.TotalScenes.size(); i++)
	{
		content += handler.TotalScenes[i] + "\n";
	}
	const std::string path = PATH_TO_SCENE_FILE;
	spe::Utility::CreateFileWithContent(content, path);
}

void spe::Savesystem::UpdateSpriteFile(const spe::SpriteRepository& repo)
{
	std::fstream spriteFile;
	spriteFile.open(PATH_TO_SPRITES, std::ios::out);

	if (spriteFile.is_open())
	{
		spriteFile << "name;vecpos;transformPosX;transformPosY;ScaleX;ScaleY;rotation;filepath;boxColliderWidthLeftOrRightX;boxColliderWidthLeftOrRighY;boxColliderHeightUpOrDownX;boxColliderHeightUpOrDownY;boxColliderExists;solid;sortingLayer;gravity;mass;physicsBodyExists;id;parentId;nextPosX;nextPosY;lastPosX;lastPosY;listPos;highestChild;positionToParentX;positionToParentY;animatorExists;prefabExist;loadInMemory;pathToPrefab;tag;lightExist;lightRadius;lightIntensity;render" << "\n";
		const std::list<spe::Sprite*>& sprites = repo.GetSpritesC();

		for (auto it = sprites.begin(); it != sprites.end(); ++it)
		{
			spe::Sprite* sprite = *it;

			std::string s;
			s.push_back(PREFAB_DELIMITER);

			std::string line = "S" + s + spe::Savesystem::GetPropertyLineWithSeperator(sprite);

			spriteFile << line << "\n";

			for (const auto& anim : sprite->Animator.Animations)
			{
				std::string animline = "A" + s + anim.second.GetPath();

				spriteFile << animline << "\n";
			}

		}
		spriteFile.close();
	}
}

void spe::Savesystem::UpdateBackgroundFile(const spe::Vector3& bg)
{
	std::fstream backgroundFile;

	backgroundFile.open(PATH_TO_BACKGROUND, std::ios::out);

	if (backgroundFile.is_open())
	{
		backgroundFile << "Red;Blue;Green" << "\n";

		std::string line = std::to_string(bg.X) + ";" + std::to_string(bg.Y) + ";" + std::to_string(bg.Z);

		backgroundFile << line << "\n";

		backgroundFile.close();
	}
}

void spe::Savesystem::UpdateHighestIndexFile(uint32_t idx)
{
	std::fstream indexFile;

	indexFile.open(PATH_TO_HIGHEST_INDEX, std::ios::out);
	if (indexFile.is_open())
	{
		indexFile << "highestIndex" << "\n";

		indexFile << idx << "\n";

		indexFile.close();
	}
}

void spe::Savesystem::UpdateCameraFile(const spe::Camera& camera)
{
	std::fstream backgroundFile;

	backgroundFile.open(PATH_TO_CAMERA, std::ios::out);

	if (backgroundFile.is_open())
	{
		backgroundFile << "TransformPoxX;TransformPosY;Zoom;Speed" << "\n";

		std::string line = std::to_string(camera.Position.X) + ";" +
			std::to_string(camera.Position.Y) + ";" +
			std::to_string(camera.GetZoom()) + ";" +
			std::to_string(camera.CameraSpeed);

		backgroundFile << line << "\n";

		backgroundFile.close();
	}
}

void spe::Savesystem::UpdateTagsFile(const spe::SpriteRepository& repo)
{
	std::fstream tag_file;
	tag_file.open(PATH_TO_TAG_FILE, std::ios::out);

	if (tag_file.is_open())
	{
		tag_file << "Tags" << "\n";

		for (int i = 0; i < repo.Tags.size(); i++)
		{
			tag_file << repo.Tags[i] << "\n";
		}

		tag_file.close();
	}
}

void spe::Savesystem::UpdateAnimationFile(const spe::SpriteRepository& repo)
{
	const std::list<spe::Sprite*>& sprites = repo.GetSpritesC();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		const spe::Sprite* sprite = *it;
		for (const auto& anim : sprite->Animator.Animations)
		{
			spe::Savesystem::CreateAnimationSaveFile(sprite, anim.second);
		}
	}
}

bool spe::Savesystem::s_CanSave = true;