#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <calculationComponents/vector2.h>

namespace s2d
{
    class Sprite;
    class SpriteRepository
    {
    private:
        std::vector<std::unique_ptr<s2d::Sprite*>> m_sprites;
    public:

        SpriteRepository() { }
    
        size_t amount() const { return this->m_sprites.size(); }
        s2d::Sprite* const readAt(size_t idx);
        const s2d::Sprite* const readAt(size_t idx, bool constConst) const;
        void deleteAt(size_t idx);
        void add(s2d::Sprite* value);
    };
}

