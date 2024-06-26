#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

namespace spe
{
	class Log
	{
	private:
		static std::string* s_m_ptr_LogBuffer;

	public:
		Log() = delete;

		static void LogString(const std::string& name);
		static void SetStringBuffer(std::string* buffer);
	};

}

