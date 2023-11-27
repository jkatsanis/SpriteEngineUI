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

		static std::string BoolToStr(bool b);
		static void CreateFileWithContent(const std::string& content, const std::string& pathAndName);

		static bool Contains(const std::string& str, const std::vector<std::string>& arr);
		static std::string VectorToString(const std::vector<std::string>& vec, char del, const std::string& ext);
		static std::string RenamePartOnPath(const std::string& path, const std::string& new_name, char del, const std::string& ext, uint32_t pos);
		static void SetCurrentDir(const std::string& path);
		static std::string getUserProjectPathSeperatetFromEnginePath(const std::string& path);
		static std::vector<std::string> Split(const std::string& s, char delim);
		static void WriteFile(const std::string& name, const std::string& path);

		static std::string GetFileExtension(const std::string& file);
		static std::string GetNamePathSplit(std::string path);
		static void Delete(const std::string& path);

		/// <summary>
		/// Should only be used 1 time. 
		/// </summary>
		static std::string GetDefaultDir();

		static std::string GetCurrentDir();

		static bool IsStringValid(const std::string& path);

		static bool IsFolder(const std::string& path);

		static std::string RemoveExtension(const std::string& file);

		static std::string CopyDir(const std::string& inputDir, const std::string& outputdir, const std::string& name);

	};
}