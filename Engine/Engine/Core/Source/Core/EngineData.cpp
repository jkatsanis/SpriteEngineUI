#include "EngineData.h"

std::string spe::EngineData::s_PathUserProject = "";
std::string spe::EngineData::s_PathOfEngine = "";
std::string spe::EngineData::s_NameOfUser = "";
std::string spe::EngineData::s_Scene = "";
bool spe::EngineData::s_BuildDebug = true;
bool spe::EngineData::s_BuildRelease = false;

void spe::EngineData::BuildProjectFiles()
{
	const std::string current = spe::Utility::GetCurrentDir();

	spe::Utility::SetCurrentDir("Build");

	spe::Log::LogString("Building project files..");
	spe::Log::LogString(spe::Utility::RunCommand("cmake .."));
	spe::Log::LogString("========FINISHED==========");

	spe::Utility::SetCurrentDir(current);
}

void spe::EngineData::BuildProject()
{
    const std::string current = spe::Utility::GetCurrentDir();

    spe::Utility::SetCurrentDir("Build");

    spe::Log::LogString("Building project files..");
    spe::Log::LogString(spe::Utility::RunCommand("cmake .."));
    spe::Log::LogString("========FINISHED==========");

    if (spe::EngineData::s_BuildRelease)
    {
        spe::Log::LogString("Compiling in Release..");
        spe::Log::LogString(spe::Utility::RunCommand("cmake --build . --config Release"));
        spe::Log::LogString("========FINISHED======");         
    }

    if (spe::EngineData::s_BuildDebug) {
        spe::Log::LogString("Compiling in Debug..");
        spe::Log::LogString(spe::Utility::RunCommand("cmake --build . --config Debug"));
        spe::Log::LogString("========FINISHED======");
    }

	spe::Utility::SetCurrentDir(current);
}
