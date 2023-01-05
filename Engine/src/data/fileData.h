#pragma once

#include <iostream>

namespace s2d
{
    class FileData
    {
    public:
        bool isFolder;
        int splitPahStringSize;

        std::string name;
        std::string path;

        FileData(bool isFolder, int splitPahStringSize, std::string name, std::string path);
    };
}