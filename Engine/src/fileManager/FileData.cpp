#include "FileData.h"

s2d::FileData::FileData(bool isFolder, int splitPahStringSize, std::string name, std::string path)
    :isFolder(isFolder), name(name), path(path), splitPahStringSize(splitPahStringSize) { }
