#pragma once

#include <iostream>
#include <vector>

namespace std
{
	static bool isTherAnotherFilter(const std::string& word, const std::string& filter, int idx)
	{
		int filterCnt = 0;
		for (int i = idx; i < word.size(); i++)
		{
			if (word[i] == filter[filterCnt])
			{
				filterCnt++;
			}
			else
			{
				filterCnt = 0;
			}
			if (filterCnt == filter.size())
			{
				return true;
			}
		}
		return false;
	}

	static std::string splitStringTillLastWord(const std::string& word, const std::string& filter)
	{
		std::string str;
		int filterCnt = 0;
		for (int i = 0; i < word.size(); i++)
		{
			if (word[i] == filter[filterCnt])
			{
				filterCnt++;
			}
			else
			{
				filterCnt = 0;
			}
			if (filterCnt == filter.size() && !isTherAnotherFilter(word, filter, i))
			{

				for (int j = i + 2; j < word.size(); j++)
				{
					str += word[j];
				}

			}
		}
		return str;
	}

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


	static vector<std::string> splitString(std::string s, std::string delimiter)
	{
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		string token;
		vector<string> res;

		while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
			token = s.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(s.substr(pos_start));
		return res;
	}
}

