#include "spriteRepository.h"
#include <physicalComponents/sprite.h>

// Constructor / Destructor

s2d::SpriteRepository::SpriteRepository()
{
    this->current_tool = s2d::EditorTools::PositionTool;
    this->sprited_hovered_in_hierarchy = nullptr;
    this->sprite_in_inspector = nullptr;
    this->child_to_parent = nullptr;
    this->right_clicked_sprite = nullptr;
}

s2d::SpriteRepository::~SpriteRepository()
{
    for(size_t i = 0; i < this->m_sprites.size(); i++)
    {
        delete this->m_sprites[i];
    }
}

// Public functions

s2d::Sprite* const s2d::SpriteRepository::readAt(size_t idx)
{
    if (idx >= this->m_sprites.size())
    {
        std::cout << "LOG [ERROR] Cant read at a invalid position";
        return nullptr;
    }
    s2d::Sprite* const ptr = this->m_sprites[idx];
    return ptr;
}

const s2d::Sprite* const s2d::SpriteRepository::readAt(size_t idx, bool constConst) const
{
    const s2d::Sprite* ptr = this->m_sprites[idx];
    return ptr;
}

void s2d::SpriteRepository::deleteAt(size_t idx)
{
    s2d::Sprite* ptr_delete = this->m_sprites[idx];
    this->m_sprites.erase(this->m_sprites.begin() + idx);

    // Destructor call
    delete ptr_delete;
}

void s2d::SpriteRepository::deleteWithName(const std::string& name)
{
    this->deleteAt(getIndexAtName(name));
}

void s2d::SpriteRepository::add(s2d::Sprite* ptr)
{
    this->m_sprites.push_back(ptr);
}

s2d::Sprite* s2d::SpriteRepository::getSpriteWithName(const std::string& name)
{
    for (size_t i = 0; i < this->m_sprites.size(); i++)
    {
        s2d::Sprite* sprite = this->m_sprites[i];
        if (sprite->name == name)
        {
            return sprite;
        }
    }

    return nullptr;
}

// Privat functions

size_t s2d::SpriteRepository::getIndexAtName(const std::string& name) const
{
    for (size_t i = 0; i < this->m_sprites.size(); i++)
    {
        const s2d::Sprite* sprite = this->m_sprites[i];
        if (sprite->name == name)
        {
            return i;
        }
    }

    return -1;
}

