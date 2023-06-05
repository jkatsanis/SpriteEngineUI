#include "UIIconData.h"

s2d::UIIconData::UIIconData()
{
    this->m_textures = std::vector<sf::Texture>(ICONS_AMOUNT);
    this->m_ids = std::vector<uint32_t>(ICONS_AMOUNT);

    for (int i = 0; i < ICONS_AMOUNT; i++)
    {
        this->m_textures[i].loadFromFile(s2d::UIInfo::s_icons[i]);
        this->m_ids[i] = this->m_textures[i].getNativeHandle();
    }
}

uint32_t s2d::UIIconData::getId(std::string fileExtension)
{
    if (fileExtension == "folder")
    {
        return this->m_ids[1];
    }
    else if (fileExtension == "prfb")
    {
        return this->m_ids[2];
    }
    else if (fileExtension == "cpp" || fileExtension == "h")
    {
        return this->m_ids[3];
    }
    else if (fileExtension == "png")
    {
        return this->m_ids[4];
    }
    else
    {
        return this->m_ids[0];
    }
    return 0;
}
