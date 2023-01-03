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
			if (c == '.' || c == '/')
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

}

