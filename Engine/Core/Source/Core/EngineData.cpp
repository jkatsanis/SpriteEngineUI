#include "EngineData.h"

std::string spe::EngineData::s_PathUserProject = "";
std::string spe::EngineData::s_PathOfEngine = "";
std::string spe::EngineData::s_NameOfUser = "";
std::string spe::EngineData::s_Scene = "";

void spe::EngineData::BuildProjectFiles()
{
	const std::string current = spe::Utility::GetCurrentDir();

	spe::Utility::SetCurrentDir("Build");

	spe::Log::LogString("Building project files..");
	system("cmake ..");
	spe::Log::LogString("========FINISHED==========");

	spe::Utility::SetCurrentDir(current);
}

void spe::EngineData::BuildProject()
{
	const std::string current = spe::Utility::GetCurrentDir();

	spe::Utility::SetCurrentDir("Build");

	spe::Log::LogString("Building project files..");
	system("cmake ..");
	spe::Log::LogString("========FINISHED==========");

	spe::Log::LogString("Compiling in Release..");
	system("cmake --build . --config Release");
	spe::Log::LogString("========FINISHED======");

	spe::Log::LogString("Compiling in Debug..");
	system("cmake --build . --config Debug");
	spe::Log::LogString("========FINISHED====");

	spe::Utility::SetCurrentDir(current);
}
