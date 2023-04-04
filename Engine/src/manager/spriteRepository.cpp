#include "spriteRepository.h"

// Public functions
    
s2d::Sprite* const s2d::SpriteRepository::readAt(size_t idx)
{
    s2d::Sprite* ptr = *this->m_sprites[idx].get();
    return ptr;
}

const s2d::Sprite* const s2d::SpriteRepository::readAt(size_t idx, bool constConst) const
{
    const s2d::Sprite* ptr = *this->m_sprites[idx].get();
    return ptr;
}

void s2d::SpriteRepository::deleteAt(size_t idx)
{
    this->m_sprites.erase(this->m_sprites.begin() + idx);
}

void s2d::SpriteRepository::add(s2d::Sprite* value)
{
    this->m_sprites.push_back(std::make_unique<s2d::Sprite*>(value));
}


