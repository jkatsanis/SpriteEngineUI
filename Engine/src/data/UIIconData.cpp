#include "UIIconData.h"

s2d::UIIconData::UIIconData()
{
    this->textureFile.loadFromFile(s2d::UIInfo::sdefaultIcon);
    this->textureFolder.loadFromFile(s2d::UIInfo::sfolderIcon);

    this->textureIDFodler = ImTextureID(this->textureFolder.getNativeHandle());
    this->textureIDFile = ImTextureID(this->textureFile.getNativeHandle());
}

ImTextureID s2d::UIIconData::getId(std::string fileExtension)
{
    if (fileExtension == "folder")
    {
        return this->textureIDFodler;
    }
    else
    {
        return this->textureIDFile;
    }
    return ImTextureID();
}
