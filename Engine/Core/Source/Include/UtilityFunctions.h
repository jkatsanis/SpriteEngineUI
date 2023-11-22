#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <GameWindows.h>

namespace spe
{
	class Utility
	{
	public:
		Utility() = delete;

		static std::vector<std::string> Split(const std::string& s, char delim);
		static void WriteFile(const std::string& name, const std::string& path);
		static std::string GetDefaultPath();
	};
}