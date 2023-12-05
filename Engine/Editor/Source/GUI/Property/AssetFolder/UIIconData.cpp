#include "UIIconData.h"

spe::UIIconData::UIIconData()
{
    const std::vector<std::string> icons =
    {
        PATH_TO_RESSOURCES"\\Icons\\defaultIcon.png",
        PATH_TO_RESSOURCES"\\Icons\\assetFolder.png",
        PATH_TO_RESSOURCES"\\Icons\\prefabIcon.png",
        PATH_TO_RESSOURCES"\\Icons\\codeIcon.png",
        PATH_TO_RESSOURCES"\\Icons\\pngIcon.png",
    };

    this->m_Textures = std::vector<sf::Texture>(icons.size());
    this->m_IDs = std::vector<uint32_t>(icons.size());

    for (int i = 0; i < icons.size(); i++)
    {
        this->m_Textures[i].loadFromFile(icons[i]);
        this->m_IDs[i] = this->m_Textures[i].getNativeHandle();
    }
}

uint32_t spe::UIIconData::GetId(std::string fileExtension)
{
    if (fileExtension == "folder")
    {
        return this->m_IDs[1];
    }
    else if (fileExtension == "prfb")
    {
        return this->m_IDs[2];
    }
    else if (fileExtension == "cpp" || fileExtension == "h")
    {
        return this->m_IDs[3];
    }
    else if (fileExtension == "png")
    {
        return this->m_IDs[4];
    }
    else
    {
        return this->m_IDs[0];
    }
    return 0;
}
