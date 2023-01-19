#pragma once

#include <calculationComponents/vector3.h>
#include <fstream>
#include <iostream>
#include <stdExtension.h>
#include <string>

namespace s2d
{
	class FileData
	{
	public:
		FileData() = delete;

		static s2d::Vector3 windowBackground;

		static void setWindowBackground();
	};
}

