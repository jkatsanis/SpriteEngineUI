#pragma once

#include <string>
#include <filesystem>
#include <vector>

namespace spe
{
	enum class BuildPlatform
	{
		Windows,
		Linux
	};

	struct BuildConfig
	{
		bool BuildWindows = true;
		bool BuildLinux = true;
		bool CopyAssets = true;
		bool CopyEngineResources = true;
		bool CopySaves = true;
	};

	class BuildManager
	{
	public:
		BuildManager() = delete;

		static bool s_BuildWindows;
		static bool s_BuildLinux;

		static void BuildForDistribution();

		static void BuildForDistribution(const BuildConfig& config);

		static void CreateReleaseFolders(const std::string& projectPath);

		static void CopyRequiredAssets(const std::string& projectPath, const std::string& platformFolder);

		static bool BuildForPlatform(BuildPlatform platform);

		static void CopyExecutableAndLibraries(const std::string& projectPath, BuildPlatform platform);

		static std::string GetPlatformFolder(const std::string& projectPath, BuildPlatform platform);

	private:
		static void LogBuildStep(const std::string& message);
		static void LogBuildError(const std::string& message);
		static void LogBuildSuccess(const std::string& message);

		static std::string GetExecutableName(const std::string& projectPath, BuildPlatform platform);
		static std::vector<std::string> GetRequiredLibraries(BuildPlatform platform);
		static void CreateLauncherScript(const std::string& platformFolder, const std::string& execName, BuildPlatform platform);
		static std::string GetBuildDirectoryName(BuildPlatform platform);
	};
}

