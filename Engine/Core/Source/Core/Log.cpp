#include "Log.h"

// Public

void spe::Log::LogString(const std::string& msg)
{
	std::cout << msg << std::endl;

	if (spe::Log::s_m_ptr_LogBuffer != nullptr)
	{
		*spe::Log::s_m_ptr_LogBuffer += msg + "\n";
		*spe::Log::s_m_ptr_LogBuffer += msg + "\n";
		*spe::Log::s_m_ptr_LogBuffer += msg + "\n";
		*spe::Log::s_m_ptr_LogBuffer += msg + "\n";
	}
}

void spe::Log::SetStringBuffer(std::string* buffer)
{
	spe::Log::s_m_ptr_LogBuffer = buffer;
}

// Static Private Init

std::string* spe::Log::s_m_ptr_LogBuffer = nullptr;