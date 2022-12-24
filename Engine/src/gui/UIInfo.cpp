#include "UIInfo.h"

float s2d::UIInfo::sdefaultFontSize = 1.0f;
bool s2d::UIInfo::srenderAssetFolder = true;

std::string s2d::UIInfo::sfolderIconPath = "EngineAssets\\Icons\\assetFolder.png";
std::string s2d::UIInfo::sdefaultIcon = "EngineAssets\\Icons\\defaultIcon.png";

sf::Texture s2d::UIInfo::textureFolder;
ImTextureID s2d::UIInfo::textureIdFolder;

sf::Texture s2d::UIInfo::file;
ImTextureID s2d::UIInfo::fileId;

void s2d::UIInfo::Init()
{
    textureFolder = sf::Texture();
    textureFolder.loadFromFile(s2d::UIInfo::sfolderIconPath);

    textureIdFolder = (ImTextureID)textureFolder.generateMipmap();
}

void s2d::UIInfo::Ini()
{
    file = sf::Texture();
    file.loadFromFile(s2d::UIInfo::sdefaultIcon);
    fileId = (ImTextureID)file.generateMipmap();

}