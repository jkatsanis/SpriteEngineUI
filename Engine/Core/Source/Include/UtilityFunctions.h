#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <Windows.h>

#include "Core/EngineData.h"
#include "Core/Log.h"

namespace spe
{
	class Utility
	{
	public:
		Utility() = delete;

		static void SetCurrentDir(const std::string& path);
		static std::string getUserProjectPathSeperatetFromEnginePath(const std::string& path);
		static std::vector<std::string> Split(const std::string& s, char delim);
		static void WriteFile(const std::string& name, const std::string& path);

		static std::string GetFileExtension(const std::string& file);

		/// <summary>
		/// Should only be used 1 time. 
		/// </summary>
		static std::string GetDefaultDir();

		static std::string GetCurrentDir();

		static std::string CopyDir(const std::string& inputDir, const std::string& outputdir, const std::string& name);

	};
}