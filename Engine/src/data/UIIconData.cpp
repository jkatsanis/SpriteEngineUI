#include "UIIconData.h"

s2d::UIIconData::UIIconData()
{
    this->m_textures = std::vector<sf::Texture>(ICONS_AMOUNT);
    this->m_ids = std::vector<uint32_t>(ICONS_AMOUNT);

    const std::string icons[ICONS_AMOUNT] = {
       s2d::UIInfo::s_folderIcon,
       s2d::UIInfo::s_prefabIcon,
       s2d::UIInfo::s_codeIcon,
       s2d::UIInfo::s_defaultIcon,
    };

    for (int i = 0; i < ICONS_AMOUNT; i++)
    {
        this->m_textures[i].loadFromFile(icons[i]);
        this->m_ids[i] = this->m_textures[i].getNativeHandle();
    }
}

uint32_t s2d::UIIconData::getId(std::string fileExtension)
{
    if (fileExtension == "folder")
    {
        return this->m_ids[0];
    }
    else if (fileExtension == "prfb")
    {
        return this->m_ids[1];
    }
    else if (fileExtension == "cpp")
    {
        return this->m_ids[2];
    }
    else
    {
        return this->m_ids[3];
    }
    return 0;
}
