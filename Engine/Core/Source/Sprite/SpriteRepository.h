#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>

#include "Core/Repository.h"
#include "Math/Vector2.h"

namespace spe
{
    class Sprite;
    class SpriteRepository : public spe::Repository<spe::Sprite>
    {
    private:
        std::list<spe::Sprite*> m_sprites;

        uint32_t getIdWithName(const std::string& name) const;
        void addChildsToDelete(std::vector<spe::Sprite*>& childs, spe::Sprite* parent);
        void eraseWithIdx(uint32_t id);
        void sortSpritesByLayer(spe::Sprite* spr);

    public:
        std::vector<std::string> Tags;
      
        bool* isFullScreened;
        bool main_content_iniitialied;

        SpriteRepository();
        ~SpriteRepository();

        // Add / delete
        void DelteWithId(uint32_t id);
        void DeleteWithName(const std::string& name);
        void Add(spe::Sprite* ref) override;

        // Utility
        void UpdateLayerIndex() override;
        void ReloadTextures();
        void cleanUp();
        void SetHighestId(uint32_t id) { this->m_HighestId = id; }

        // getter
        uint32_t GetListIndex(spe::Sprite* sprite);
        std::list<spe::Sprite*>& GetSprites() { return this->m_sprites; }
        uint32_t GetAmount() const override { return (uint32_t)this->m_sprites.size(); }
        spe::Sprite* GetById(uint32_t idx) override;
        spe::Sprite* GetByName(const std::string& name) override;
        uint32_t GetHighestLayer() const { return this->m_HighestLayer; }
        uint32_t GetHighestId() const { return this->m_HighestId; }

    public:
        static void getAllChilds(std::vector<const spe::Sprite*>& childs, const spe::Sprite* parent);
        static spe::Sprite* getWithId(std::vector<spe::Sprite*>& collection, uint32_t id);
    };
}

