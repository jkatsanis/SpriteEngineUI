#include "BuildManager.h"

#include <fstream>

#include "Log.h"
#include "UtilityFunctions.h"

namespace fs = std::filesystem;

#ifdef __linux__
bool spe::BuildManager::s_BuildWindows = false;
bool spe::BuildManager::s_BuildLinux = true;
#else
bool spe::BuildManager::s_BuildWindows = true;
bool spe::BuildManager::s_BuildLinux = false;
#endif

void spe::BuildManager::BuildForDistribution()
{
	BuildConfig config;
	config.BuildWindows = s_BuildWindows;
	config.BuildLinux = s_BuildLinux;
	BuildForDistribution(config);
}

void spe::BuildManager::BuildForDistribution(const BuildConfig& config)
{
	
	
	const std::string currentDir = spe::Utility::GetCurrentDir();

	fs::path projectPath;
	try
	{
		projectPath = fs::canonical(currentDir);
	}
	catch (const fs::filesystem_error& e)
	{
		LogBuildError("Failed to resolve project path: " + std::string(e.what()));
		return;
	}

	const std::string resolvedPath = projectPath.string();

	LogBuildStep("========================================");
	LogBuildStep("Starting Build for Distribution...");
	LogBuildStep("Project: " + resolvedPath);
	LogBuildStep("========================================");

	
	LogBuildStep("Creating release folders...");
	CreateReleaseFolders(resolvedPath);

	
#ifdef __linux__
	LogBuildStep("Building for Linux...");
	if (BuildForPlatform(BuildPlatform::Linux))
	{
		CopyExecutableAndLibraries(resolvedPath, BuildPlatform::Linux);
		if (config.CopyAssets || config.CopyEngineResources || config.CopySaves)
		{
			CopyRequiredAssets(resolvedPath, GetPlatformFolder(resolvedPath, BuildPlatform::Linux));
		}
		LogBuildSuccess("Linux build completed successfully!");
	}
	else
	{
		LogBuildError("Linux build failed!");
	}
#else
	LogBuildStep("Building for Windows...");
	if (BuildForPlatform(BuildPlatform::Windows))
	{
		CopyExecutableAndLibraries(resolvedPath, BuildPlatform::Windows);
		if (config.CopyAssets || config.CopyEngineResources || config.CopySaves)
		{
			CopyRequiredAssets(resolvedPath, GetPlatformFolder(resolvedPath, BuildPlatform::Windows));
		}
		LogBuildSuccess("Windows build completed successfully!");
	}
	else
	{
		LogBuildError("Windows build failed!");
	}
#endif

	LogBuildStep("========================================");
	LogBuildStep("Build for Distribution Complete!");
	LogBuildStep("========================================");
}

void spe::BuildManager::CreateReleaseFolders(const std::string& projectPath)
{
	
	fs::path absolutePath = fs::weakly_canonical(projectPath);

	
#ifdef __linux__
	std::string platform = "Linux";
#else
	std::string platform = "Windows";
#endif

	try
	{
		
		fs::remove_all(absolutePath / "Release");

		fs::path platformBase = absolutePath / "Release" / platform;
		fs::create_directories(platformBase / "Assets");
		fs::create_directories(platformBase / "Engine" / "Ressources");
		fs::create_directories(platformBase / "Engine" / "Saves");
		fs::create_directories(platformBase / "Executable");
		LogBuildStep("Created folder structure for: " + platform);
	}
	catch (const fs::filesystem_error& e)
	{
		LogBuildError("Failed to create release folders: " + std::string(e.what()));
	}
}

void spe::BuildManager::CopyRequiredAssets(const std::string& projectPath, const std::string& platformFolder)
{
	
	fs::path absoluteProjectPath = fs::weakly_canonical(projectPath);
	fs::path absolutePlatformFolder = fs::weakly_canonical(platformFolder);

	LogBuildStep("Copying assets to: " + absolutePlatformFolder.string());

	
	fs::path assetsSource = absoluteProjectPath / "Assets";
	fs::path assetsDest = absolutePlatformFolder / "Assets";

	if (fs::exists(assetsSource))
	{
		try
		{
			
			fs::create_directories(assetsDest);

			
			for (const auto& entry : fs::directory_iterator(assetsSource))
			{
				const std::string filename = entry.path().filename().string();

				
				if (filename == "Scripts")
				{
					LogBuildStep("Skipping Assets/Scripts (source code already compiled)");
					continue;
				}

				fs::path destPath = assetsDest / filename;

				if (fs::is_directory(entry.path()))
				{
					fs::copy(entry.path(), destPath, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
				}
				else
				{
					fs::copy_file(entry.path(), destPath, fs::copy_options::overwrite_existing);
				}
			}
			LogBuildStep("Copied Assets folder (excluding Scripts)");
		}
		catch (const fs::filesystem_error& e)
		{
			LogBuildError("Failed to copy Assets: " + std::string(e.what()));
		}
	}
	else
	{
		LogBuildStep("No Assets folder found to copy");
	}

	
	fs::path ressourcesSource = absoluteProjectPath / "Engine" / "Ressources";
	fs::path ressourcesDest = absolutePlatformFolder / "Engine" / "Ressources";

	if (fs::exists(ressourcesSource))
	{
		try
		{
			fs::create_directories(ressourcesDest);
			fs::copy(ressourcesSource, ressourcesDest, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
			LogBuildStep("Copied Engine/Ressources folder");
		}
		catch (const fs::filesystem_error& e)
		{
			LogBuildError("Failed to copy Engine/Ressources: " + std::string(e.what()));
		}
	}
	else
	{
		LogBuildStep("No Engine/Ressources folder found to copy");
	}

	
	fs::path savesSource = absoluteProjectPath / "Engine" / "Saves";
	fs::path savesDest = absolutePlatformFolder / "Engine" / "Saves";

	if (fs::exists(savesSource))
	{
		try
		{
			fs::create_directories(savesDest);
			fs::copy(savesSource, savesDest, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
			LogBuildStep("Copied Engine/Saves folder");
		}
		catch (const fs::filesystem_error& e)
		{
			LogBuildError("Failed to copy Engine/Saves: " + std::string(e.what()));
		}
	}
	else
	{
		LogBuildStep("No Engine/Saves folder found to copy");
	}
}

bool spe::BuildManager::BuildForPlatform(BuildPlatform platform)
{
	
	const std::string currentDir = spe::Utility::GetCurrentDir();

	
	fs::path projectPath;
	try
	{
		projectPath = fs::canonical(currentDir);
	}
	catch (const fs::filesystem_error& e)
	{
		LogBuildError("Failed to resolve project path: " + std::string(e.what()));
		return false;
	}

	
	std::string buildDirName = GetBuildDirectoryName(platform);
	const std::string buildDir = (projectPath / buildDirName).string();

	
	if (!fs::exists(buildDir))
	{
		try
		{
			fs::create_directories(buildDir);
			LogBuildStep("Created build directory: " + buildDir);
		}
		catch (const fs::filesystem_error& e)
		{
			LogBuildError("Failed to create build directory: " + std::string(e.what()));
			return false;
		}
	}

	
	try
	{
		spe::Utility::SetCurrentDir(buildDir);
	}
	catch (const std::runtime_error& e)
	{
		LogBuildError("Failed to change to build directory: " + std::string(e.what()));
		return false;
	}

	std::string cmakeConfigCommand;
	std::string cmakeBuildCommand;

#ifdef __linux__
	cmakeConfigCommand = "cmake .. -DCMAKE_BUILD_TYPE=Release";
	cmakeBuildCommand = "cmake --build . --config Release -j$(nproc)";
#else
	cmakeConfigCommand = "cmake .. -DCMAKE_BUILD_TYPE=Release";
	cmakeBuildCommand = "cmake --build . --config Release";
#endif

	LogBuildStep("Running: " + cmakeConfigCommand);
	std::string configOutput = spe::Utility::RunCommand(cmakeConfigCommand.c_str());
	spe::Log::LogString(configOutput);

	LogBuildStep("Running: " + cmakeBuildCommand);
	std::string buildOutput = spe::Utility::RunCommand(cmakeBuildCommand.c_str());
	spe::Log::LogString(buildOutput);

	
	try
	{
		spe::Utility::SetCurrentDir(projectPath.string());
	}
	catch (const std::runtime_error& e)
	{
		LogBuildError("Failed to restore directory: " + std::string(e.what()));
	}

	
	std::string execName = GetExecutableName(projectPath.string(), platform);
	fs::path execPath = fs::path(buildDir) / "Release" / execName;

	
	if (!fs::exists(execPath))
	{
		execPath = fs::path(buildDir) / execName;
	}

	if (!fs::exists(execPath))
	{
		LogBuildError("Expected executable not found: " + execName);
		LogBuildStep("Searched in: " + (fs::path(buildDir) / "Release").string());
		LogBuildStep("Searched in: " + buildDir);
	}

	return fs::exists(execPath);
}

void spe::BuildManager::CopyExecutableAndLibraries(const std::string& projectPath, BuildPlatform platform)
{
	
	fs::path absoluteProjectPath = fs::weakly_canonical(projectPath);
	fs::path buildDir = absoluteProjectPath / GetBuildDirectoryName(platform);
	fs::path platformFolder = fs::weakly_canonical(GetPlatformFolder(projectPath, platform));
	fs::path executableFolder = platformFolder / "Executable";
	const std::string execName = GetExecutableName(projectPath, platform);

	
	fs::create_directories(executableFolder);

	
	fs::path execSource = buildDir / "Release" / execName;
	if (!fs::exists(execSource))
	{
		execSource = buildDir / execName;
	}

	if (fs::exists(execSource))
	{
		try
		{
			fs::copy_file(execSource, executableFolder / execName,
				fs::copy_options::overwrite_existing);
			LogBuildStep("Copied executable: " + execName);
		}
		catch (const fs::filesystem_error& e)
		{
			LogBuildError("Failed to copy executable: " + std::string(e.what()));
		}
	}
	else
	{
		LogBuildError("Executable not found: " + execSource.string());
	}

	
	std::vector<std::string> libs = GetRequiredLibraries(platform);
	fs::path sfmlLibDir = buildDir / "ThirdParty" / "sfml" / "lib";

	for (const std::string& lib : libs)
	{
		fs::path libSource = sfmlLibDir / "Release" / lib;
		if (!fs::exists(libSource))
		{
			libSource = sfmlLibDir / lib;
		}

		if (fs::exists(libSource))
		{
			try
			{
				fs::copy_file(libSource, executableFolder / lib,
					fs::copy_options::overwrite_existing);
				LogBuildStep("Copied library: " + lib);
			}
			catch (const fs::filesystem_error& e)
			{
				LogBuildError("Failed to copy library " + lib + ": " + std::string(e.what()));
			}
		}
	}

	
	CreateLauncherScript(platformFolder.string(), execName, platform);
}

std::string spe::BuildManager::GetPlatformFolder(const std::string& projectPath, BuildPlatform platform)
{
	
	fs::path absolutePath = fs::weakly_canonical(projectPath);
	std::string platformName = (platform == BuildPlatform::Windows) ? "Windows" : "Linux";
	return (absolutePath / "Release" / platformName).string();
}

void spe::BuildManager::CreateLauncherScript(const std::string& platformFolder, const std::string& execName, BuildPlatform platform)
{
	fs::path platformPath = fs::weakly_canonical(platformFolder);

	try
	{
		if (platform == BuildPlatform::Linux)
		{
			
			
			std::string baseName = execName;
			fs::path launcherPath = platformPath / ("run_" + baseName + ".sh");

			std::ofstream launcher(launcherPath);
			if (launcher.is_open())
			{
				launcher << "#!/bin/bash\n";
				launcher << "# Launcher script for " << execName << "\n";
				launcher << "# This script ensures the game runs with the correct working directory\n\n";
				launcher << "SCRIPT_DIR=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")\" && pwd)\"\n";
				launcher << "cd \"$SCRIPT_DIR/Executable\"\n";  
				launcher << "export LD_LIBRARY_PATH=\"$SCRIPT_DIR/Executable:$LD_LIBRARY_PATH\"\n";
				launcher << "./" << execName << " \"$@\"\n";
				launcher.close();

				
				fs::permissions(launcherPath, fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec, fs::perm_options::add);

				LogBuildStep("Created launcher script: run_" + baseName + ".sh");
			}
			else
			{
				LogBuildError("Failed to create launcher script");
			}
		}
		else if (platform == BuildPlatform::Windows)
		{
			
			std::string baseName = execName;
			
			if (baseName.size() > 4 && baseName.substr(baseName.size() - 4) == ".exe")
			{
				baseName = baseName.substr(0, baseName.size() - 4);
			}
			fs::path launcherPath = platformPath / ("run_" + baseName + ".bat");

			std::ofstream launcher(launcherPath);
			if (launcher.is_open())
			{
				launcher << "@echo off\n";
				launcher << "REM Launcher script for " << execName << "\n";
				launcher << "REM This script ensures the game runs with the correct working directory\n\n";
				launcher << "cd /d \"%~dp0Executable\"\n";  
				launcher << execName << " %*\n";
				launcher.close();

				LogBuildStep("Created launcher script: run_" + baseName + ".bat");
			}
			else
			{
				LogBuildError("Failed to create launcher script");
			}
		}
	}
	catch (const std::exception& e)
	{
		LogBuildError("Failed to create launcher script: " + std::string(e.what()));
	}
}

void spe::BuildManager::LogBuildStep(const std::string& message)
{
	spe::Log::LogString("[BUILD] " + message);
}

void spe::BuildManager::LogBuildError(const std::string& message)
{
	spe::Log::LogString("[BUILD ERROR] " + message);
}

void spe::BuildManager::LogBuildSuccess(const std::string& message)
{
	spe::Log::LogString("[BUILD SUCCESS] " + message);
}

std::string spe::BuildManager::GetExecutableName(const std::string& projectPath, BuildPlatform platform)
{
	
	fs::path absolutePath = fs::weakly_canonical(projectPath);
	std::string projectName = absolutePath.filename().string();

	
	if (projectName.empty())
	{
		projectName = absolutePath.parent_path().filename().string();
	}

	if (platform == BuildPlatform::Windows)
	{
		return projectName + ".exe";
	}
	else
	{
		return projectName;
	}
}

std::vector<std::string> spe::BuildManager::GetRequiredLibraries(BuildPlatform platform)
{
	std::vector<std::string> libs;

	if (platform == BuildPlatform::Windows)
	{
		libs = {
			"sfml-audio-2.dll",
			"sfml-graphics-2.dll",
			"sfml-network-2.dll",
			"sfml-system-2.dll",
			"sfml-window-2.dll"
		};
	}
	else
	{
		
		
		libs = {
			"libsfml-audio.so.2.6",
			"libsfml-graphics.so.2.6",
			"libsfml-network.so.2.6",
			"libsfml-system.so.2.6",
			"libsfml-window.so.2.6"
		};
	}

	return libs;
}

std::string spe::BuildManager::GetBuildDirectoryName(BuildPlatform platform)
{
	
	return "Build";
}



