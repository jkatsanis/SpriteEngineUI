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
    class Sprite;           // The sprite id wont reset over scenes
    class SpriteRepository : public spe::Repository<spe::Sprite>
    {
    private:
        std::list<spe::Sprite*> m_Sprites;

        uint32_t GetIdWithName(const std::string& name) const;
        void AddChildsToDelete(std::vector<spe::Sprite*>& childs, spe::Sprite* parent);
        void EraseWithIdx(uint32_t id);
        void SortSpritesByLayer(spe::Sprite* spr);
        void ValidateAdd(spe::Sprite* spr);
    public:
        std::vector<std::string> Tags;

        bool* IsFullScreened;
        bool Initialized;

        SpriteRepository();
        ~SpriteRepository();

        // Add / delete
        void DelteWithId(uint32_t id);
        void DeleteWithName(const std::string& name);
        void Add(spe::Sprite* ref) override;

        void SortSpritesByLayer();

        // Utility
        void UpdateLayerIndex() override;
        void ReloadTextures();
        void cleanUp();
        void SetHighestId(uint32_t id);

        // getter
        bool ExistWithId(uint32_t id);
        uint32_t GetListIndex(spe::Sprite* sprite);
        std::list<spe::Sprite*>& GetSprites() { return  this->m_Sprites; }
        const std::list<spe::Sprite*>& GetSpritesC() const { return  this->m_Sprites; }
        uint32_t GetAmount() const override { return (uint32_t)this->m_Sprites.size(); }
        spe::Sprite* GetById(uint32_t idx) override;
        spe::Sprite* GetByName(const std::string& name) override;
        uint32_t GetHighestLayer() const { return this->m_HighestLayer; }
        uint32_t GetHighestId() const { return this->m_HighestId; }

        /// <summary>
        /// Kinda inefficent. Try not to use much
        /// </summary>
        /// <param name="layer"></param>
        /// <param name="id"></param>
        void SetSpriteSortingLayer(uint32_t layer, spe::Sprite* id);

    public:
        static void GetAllChilds(std::vector<const spe::Sprite*>& childs, const spe::Sprite* parent);
        static spe::Sprite* GetWithId(std::vector<spe::Sprite*>& collection, uint32_t id);
    };
}

