#pragma once

#include <iostream>

namespace std
{
	static bool isFolder(std::string name)
	{
		for (const char c : name)
		{
			if (c == '.')
			{
				return false;
			}
		}
		return true;
	}

	static bool isStringValid(std::string str)
	{
		int cnt = 0;
		for (const char c : str)
		{
			if (c == '.' || c == '/' || c == '$')
			{
				cnt++;
			}
		}
		return !(cnt == str.size());
	}

	static int getStringSplittedSize(std::string line, std::string seperator)
	{
		size_t pos = 0;
		int cnt = 0;
		while ((pos = line.find(seperator)) != std::string::npos) {
			std::string token = line.substr(0, pos);
			cnt++;
			line.erase(0, pos + seperator.length());
		}
		return cnt + 1;
	}

	static void splitString(std::string line, std::string seperator, std::string arr[])
	{
		int cnt = 0;

		size_t pos = 0;
		while ((pos = line.find(seperator)) != std::string::npos) {
			std::string token = line.substr(0, pos);
			arr[cnt] = token;
			cnt++;
			line.erase(0, pos + seperator.length());
		}
		arr[cnt] = line;
	}

	static std::string* splitString(std::string line, std::string seperator)
	{
		int cnt = 0;
		std::string* props = new std::string[getStringSplittedSize(line, seperator)];

		size_t pos = 0;
		while ((pos = line.find(seperator)) != std::string::npos) {
			std::string token = line.substr(0, pos);
			props[cnt] = token;
			cnt++;
			line.erase(0, pos + seperator.length());
		}
		props[cnt] = line;

		return props;
	}

	static std::string* splitString(std::string line, std::string seperator, int& size)
	{
		int cnt = 0;
		std::string* props = new std::string[getStringSplittedSize(line, seperator)];

		size_t pos = 0;
		while ((pos = line.find(seperator)) != std::string::npos) {
			std::string token = line.substr(0, pos);
			props[cnt] = token;
			cnt++;
			line.erase(0, pos + seperator.length());
		}
		props[cnt] = line;

		size = cnt;

		return props;
	}

	static std::string getFileExtension(const std::string& file)
	{
		for (int i = 0; i < file.size(); i++)
		{
			if (file[i] == '.')
			{
				bool isValid = true;
				std::string extension = "";
				for (int j = i + 1; j < file.size(); j++)
				{
					extension += file[j];
					if (file[j] == '.')
					{
						isValid = false;
						break;
					}
				}
				if (isValid)
				{
					return extension;
				}
			}
		}
		return "folder";
	}
}

