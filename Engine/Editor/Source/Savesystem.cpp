#include "Savesystem.h"

// Private


// Public 

void spe::Savesystem::SaveEverything(const spe::SpriteRepository& sprites, const spe::GUIRepository& gui, const spe::SceneHandler& scene)
{

}

void spe::Savesystem::SaveProjects(const std::vector<spe::UserProjectInfo>& projects)
{
	std::string content;

	for (const spe::UserProjectInfo& project : projects)
	{
		content += project.name + ";" + project.AbsulutePath + ";" + project.lastOpened + ";" + project.relativePath + "\n";
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

		if (!corePath.is_open())
		{
			throw std::runtime_error("Couldn't open corepath file of user project");
		}

		const std::string pathCore = spe::Utility::GetCurrentDir();
		
		corePath << pathCore << "\n";
	}

}


void spe::Savesystem::CreateAnimationSaveFile(const spe::Sprite* ptr_sprite, const spe::Animation& animationToSave)
{
	const std::string path = spe::Utility::GetCurrentDir();
	// Loading the sprites from the user directory
	spe::Utility::SetCurrentDir(spe::EngineData::s_PathUserProject);

	std::string name = animationToSave.GetName();
	std::string content =
		animationToSave.GetName() + "\n" +
		std::to_string(ptr_sprite->getId()) + "\n" +
		spe::Utility::BoolToStr(animationToSave.Loop) + "\n";

	const std::vector<spe::KeyFrame>& frames = animationToSave.GetkeyFrames();

	for (const spe::KeyFrame& frame : frames)
	{
		content += std::to_string(frame.delay) + std::string(";") +
			spe::Utility::getUserProjectPathSeperatetFromEnginePath(frame.path) + "\n";
	}

	std::string pathAndName = animationToSave.GetPath();
	spe::Utility::CreateFileWithContent(content, pathAndName);

	spe::Utility::SetCurrentDir(path); 
}