#pragma once

#include <Include/SFML/Graphics.hpp>
#include <iostream>

namespace spe
{
	class Log
	{
	public:
		Log() = delete;

		static void LogString(const std::string& name);
	};

}

