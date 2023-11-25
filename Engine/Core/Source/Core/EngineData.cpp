#include "EngineData.h"

std::string spe::EngineData::s_PathUserProject = "";
std::string spe::EngineData::s_PathOfEngine = "";
std::string spe::EngineData::s_NameOfUser = "";
std::string spe::EngineData::s_Scene = "";

void spe::EngineData::BuildProject()
{
	const std::string current = spe::Utility::GetCurrentDir();

	spe::Utility::SetCurrentDir("Build");

	system("cmake ..");
	system("cmake --build . --config Release");
	system("cmake --build . --config Debug");

	spe::Utility::SetCurrentDir(current);
}
