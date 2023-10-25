#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

namespace std
{
    std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);
    std::string vectorToPathString(const std::vector<std::string>& vec, const std::string& del, const std::string& ext);
    std::string renamePartOnPath(const std::string& path, const std::string& new_name, const std::string& del, const std::string& ext, uint32_t pos);
    std::string getFileOnPath(const std::string& path);
    std::string removeExtension(const std::string& file);
    std::string boolToStr(bool b);
    void createFileWithContent(const std::string& content, const std::string& pathAndName);
    void removeFile(const std::string& path);
    bool isTherAnotherFilter(const std::string& word, const std::string& filter, int idx);
    std::string splitStringTillLastWord(const std::string& word, const std::string& filter);
    bool isFolder(std::string name);
    bool isStringValid(std::string str);
    std::string getFileExtension(const std::string& file);

    template<class T>
    void removeAtVec(std::vector<T>& vec, int pos)
    {
        if (pos >= vec.size())
        {
            return;
        }
        vec.erase(vec.begin() + pos);
    }


    void getFilePathWithExtensionInFolder(const std::filesystem::path& path, const std::string& extension, std::vector<std::string>& to);
    bool isEqualWithAny(const std::string& str, const std::vector<std::string>& arr);
}
