#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>

#include "Math/Vector2.h"

namespace spe
{
    class Sprite;
    class SpriteRepository
    {
    private:
        std::list<spe::Sprite*> m_sprites;
        int32_t m_highestLayerIndex;

        size_t getIdWithName(const std::string& name) const;
        void addChildsToDelete(std::vector<spe::Sprite*>& childs, spe::Sprite* parent);
        void eraseWithIdx(uint32_t id);
        void sortSpritesByLayer(spe::Sprite* spr);

    public:
        uint32_t highest_sprite_id;

        bool* isFullScreened;
        bool main_content_iniitialied;

        SpriteRepository();
        ~SpriteRepository();

        size_t amount() const { return this->m_sprites.size(); }
        spe::Sprite* const getWithId(size_t idx);
        void delteWithId(size_t idx);
        void deleteWithName(const std::string& name);
        void add(spe::Sprite* ref);
        spe::Sprite* getSpriteWithName(const std::string& name);
        uint32_t getListIndex(spe::Sprite* sprite);

        void updateHighestLayerIndex();
        void reloadTextures();
        void cleanUp();

        std::list<spe::Sprite*>& getSprites() { return this->m_sprites; }

        static void getAllChilds(std::vector<const spe::Sprite*>& childs, const spe::Sprite* parent);
        static spe::Sprite* getWithId(std::vector<spe::Sprite*>& collection, uint32_t id);

        uint32_t getHighestLayerIndex() const { return this->m_highestLayerIndex; }
    };
}

