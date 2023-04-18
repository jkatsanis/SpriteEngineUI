#include "UIIconData.h"

s2d::UIIconData::UIIconData()
{
    this->textureFile.loadFromFile(s2d::UIInfo::s_defaultIcon);
    this->textureFolder.loadFromFile(s2d::UIInfo::s_folderIcon);

    this->textureIDFodler = this->textureFolder.getNativeHandle();
    this->textureIDFile =  this->textureFile.getNativeHandle();
}

uint32_t s2d::UIIconData::getId(std::string fileExtension)
{
    if (fileExtension == "folder")
    {
        return this->textureIDFodler;
    }
    else
    {
        return this->textureIDFile;
    }
    return 0;
}
